from torch.autograd import Variable
from torch.autograd import gradcheck
from torch.autograd import Function
from torch.nn.parameter import Parameter
from torch.nn.modules.conv import _ConvNd
from torch.nn.modules.utils import _pair, _single
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torch
import argparse
import torchvision.models as models
import torchvision
import torchvision.transforms as transforms
import time
import math
import argparse
import numpy as np
import sys

import util


best_prec1 = 0
LAYER_NUM = 9
numQ =  [1,     1,      1,      1,      1,      1,      1,       1,1]
scale_E = 0
MM = 0
MV = 0
gamma = 0
beta = 0
N = 0
num = 0
num_of_neg_T = 0
fW1 = 32
layer_num = 0
fc_layer_num= 0
conv_layer_num= 0

start_epoch = 0
end_epoch = 300

#===============================================================================================
#                                           Option 설정부 
#===============================================================================================

parser = argparse.ArgumentParser(description='PyTorch Binary ConvNet Training')
parser.add_argument('-r', '--restart', dest='restart', action='store_true',
                    help='train model on test set')
parser.add_argument('-t', '--train', dest='train', action='store_true',
                    help='train model on test set')
parser.add_argument('-e', '--evaluate', dest='evaluate', action='store_true',
                    help='evaluate model on validation set')
parser.add_argument('-d', '--dump', dest='dump', action='store_true',
                    help='dump param')
args = parser.parse_args()  

#===============================================================================================
#                                       Image 불러올때의 Option
#===============================================================================================

transform = transforms.Compose(
    [transforms.ToTensor(),                                         # image >> array / (0 ~ 255) >> (0 ~ 1)
     transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])       # (0 ~ 1) >> (-1 ~ 1)

transform_for_train = transforms.Compose(
    [
     transforms.Pad(4),
     transforms.RandomCrop(32),
     transforms.RandomHorizontalFlip(),
     transforms.ToTensor(),
     transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ])

trainset = torchvision.datasets.CIFAR10(root='./data', train=True,
                                        download=True, transform=transform_for_train)
trainloader = torch.utils.data.DataLoader(trainset, batch_size=1024,
                                          shuffle=True, num_workers=8)

testset = torchvision.datasets.CIFAR10(root='./data', train=False,
                                       download=True, transform=transform)
testloader = torch.utils.data.DataLoader(testset, batch_size=1024,
                                         shuffle=False, num_workers=8)

val_loader = torch.utils.data.DataLoader(
    torchvision.datasets.ImageFolder('/home/ympark/data/', transform=transform), 
    batch_size=64,     
    shuffle=False, 
    num_workers=4, 
    pin_memory=True)

classes = ('plane', 'car', 'bird', 'cat',
           'deer', 'dog', 'frog', 'horse', 'ship', 'truck')


#===============================================================================================
#                                             BCNN tool
#===============================================================================================
class BinActive(torch.autograd.Function):
    '''
    Binarize the input activations and calculate the mean across channel dimension.
    '''
    def forward(self, input):
        self.save_for_backward(input)
        size = input.size()
        input = input.clamp(0.,1.).round() * 2 - 1
        return input

    def backward(self, grad_output):
        input, = self.saved_tensors
        grad_input = grad_output.clone()
        grad_input = (input.lt(0.0) + input.gt(1.0)).float().lt(0.5).float()*grad_input
        return grad_input * 2

class BinConv2d(nn.Module): # change the name of BinConv2d
    def __init__(self, input_channels, output_channels,
            kernel_size=-1, stride=-1, padding=-1, groups=1, dropout=0,
            Linear=False):
        super(BinConv2d, self).__init__()
        self.layer_type = 'BinConv2d'
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding
        self.dropout_ratio = dropout

        if dropout!=0:
            self.dropout = nn.Dropout(dropout)
        self.Linear = Linear
        if not self.Linear:
            self.bn = nn.BatchNorm2d(input_channels, eps=1e-4, momentum=0.1, affine=True)
            self.padding = nn.ConstantPad2d(padding=padding, value =-1)
            self.conv = nn.Conv2d(input_channels, output_channels,
                    kernel_size=kernel_size, stride=stride, padding=0, groups=groups, bias = False)
        else:
            self.bn = nn.BatchNorm1d(input_channels, eps=1e-4, momentum=0.1, affine=True)
            self.linear = nn.Linear(input_channels, output_channels, bias = False)
        # self.relu = nn.ReLU(inplace=True)
    
    def forward(self, x):
        # print(x.lt(0.).sum())
        x = self.bn(x)
        x = BinActive()(x)
        if self.training == False:
            fmap_dump_CL(x,1)
        if self.dropout_ratio!=0:
            x = self.dropout(x)
        if not self.Linear:
            x = self.padding(x)
            x = self.conv(x)
        else:
            x = self.linear(x)
        # x = self.relu(x)
        return x

def initialize_weights(m):
    if (isinstance(m, nn.Conv2d) | isinstance(m, Conv2D_b)):
        # nn.init.kaiming_normal_(m.weight.data, mode='fan_in')
        w, h = m.kernel_size
        c = math.sqrt(2.0 / (w * h * m.in_channels))
        nn.init.normal_(m.weight.data, std=c)
        print(m)


#===============================================================================================
#                                             Net의 설정 
#===============================================================================================

def fmap_dump_CL(x):
    global f_fmap, layer_num
    img = x.data
    bits = img.transpose(1,2).contiguous().transpose(2,3).contiguous()
    bits = bits.cpu().float().numpy()
    for element in bits:
        f_fmap.write(element)


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv0_pad = nn.ConstantPad2d(padding=1, value=0)
        self.conv0 = nn.Conv2d(in_channels= 3, out_channels= 128, kernel_size= 3, stride=1, padding=0, bias=False)
        self.conv0_bn = nn.BatchNorm2d(num_features = 128, affine = True)
        self.conv1 = BinConv2d(128, 128, kernel_size=3, stride=1, padding=1)
        self.conv1_maxpool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv2 = BinConv2d(128, 256, kernel_size= 3, stride=1, padding=1)
        self.conv3 = BinConv2d(256, 256, kernel_size= 3, stride=1, padding=1)
        self.conv3_maxpool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv4 = BinConv2d(256, 512, kernel_size= 3, stride=1, padding=1)
        self.conv5 = BinConv2d(512, 512, kernel_size= 3, stride=1, padding=1)
        self.conv5_maxpool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv6 = BinConv2d(512, 1024, kernel_size= 4, stride=1, padding=0)
        self.conv7 = BinConv2d(1024, 1024, kernel_size= 1, stride=1, padding=0)
        self.conv7_bn = nn.BatchNorm2d(1024, affine = True)
        self.conv8 = nn.Conv2d(1024, 10, kernel_size=1, stride=1, padding=0, bias=False)

    def forward(self, x):
        if self.training == False:
            fmap_dump_CL(x,1)
        # print(x)
        # print(self.conv0.weight)
        x = self.conv0_pad(x)
        x = self.conv0(x)
        # print(x)
        x = self.conv0_bn(x)
        # print(x)
        x = F.relu(x)
        x = self.conv1(x)
        print(x)
        x = self.conv1_maxpool(x)
        x = self.conv2(x)
        x = self.conv3(x)
        x = self.conv3_maxpool(x)
        x = self.conv4(x)
        x = self.conv5(x)
        x = self.conv5_maxpool(x)
        x = self.conv6(x)
        x = self.conv7(x)
        x = self.conv7_bn(x)
        x = F.relu(x)
        x = self.conv8(x)
        if self.training == False:
            fmap_dump_CL(x,1)
        x = x.view(-1,10)
        print(F.softmax(x))
        return x

#===============================================================================================
#                                        Weight 초기화
#===============================================================================================        

def save_checkpoint(state, filename='CIFAR10_BNN_3.pth.tar'):
    torch.save(state, filename)


def abs_BN(layer):
    if (type(layer) == nn.BatchNorm2d) | (type(layer) == nn.BatchNorm1d):
        layer.weight.data = layer.weight.data.abs()

#===============================================================================================
#                                         Training 부분 
#===============================================================================================   

net = Net()                         # net의 생성 
print(net)                          # net의 정보 출력 
net = torch.nn.DataParallel(net, device_ids=[0,1]).cuda()
criterion = nn.CrossEntropyLoss().cuda()                            # Loss 함수 적용 
# optimizer = optim.SGD(net.parameters(), lr=0.1, momentum=0.9)       # SGD기법적용, weight 갱신 
optimizer = optim.Adam(net.parameters(), lr=0.001)       # SGD기법적용, weight 갱신 
for m in net.modules():
    if isinstance(m, nn.Conv2d) or isinstance(m, nn.Linear):
        c = math.sqrt(2.0/float(m.weight.data[0].nelement()))
        m.weight.data = m.weight.data.normal_(0, c)

torchvision.set_image_backend('accimage')

global bin_op
bin_op = util.BinOp(net)

if args.train:
    checkpoint = torch.load('CIFAR10_BNN_3.pth.tar')
    net.load_state_dict(checkpoint['state_dict'])
    optimizer.load_state_dict(checkpoint['optimizer'])
    start_epoch = checkpoint['epoch']


if (args.train | args.restart):
    for epoch in range(start_epoch, end_epoch):
        running_loss = 0.0
        for i, data in enumerate(trainloader, 0):
            bin_op.binarization()
            # net.apply(abs_BN)
            inputs, labels = data
            labels = labels.cuda(non_blocking=True)
            net.train()
            optimizer.zero_grad()
            outputs = net(inputs)
            inputs, labels = Variable(inputs), Variable(labels)
            loss = criterion(outputs, labels)

            loss.backward()
            bin_op.restore()
            bin_op.updateBinaryGradWeight()   
            optimizer.step()
            running_loss += loss.item()
            if i % 10 == 9:    # print every 2000 mini-batches\
                print('[%d, %5d] loss: %.3f' % (epoch + 1, i + 1, running_loss / 10))
                running_loss = 0.0
        correct = 0
        total = 0
        with torch.no_grad():
            for data in testloader:
                images, labels = data
                net.eval()
                bin_op.binarizeConvParams()
                # net.apply(abs_BN)
                outputs = net(images)
                labels = labels.cuda(non_blocking=True)
                images, labels = Variable(images), Variable(labels)
                _, predicted = torch.max(outputs.data, 1)
                total += labels.size(0)
                correct += (predicted == labels).sum().item()
                bin_op.restore()

        for param_group in optimizer.param_groups:
            lr = param_group['lr']
            if epoch < 200:
               param_group['lr'] = 0.0001
            elif epoch < 250:
               param_group['lr'] = 0.0001
            elif epoch < 300:
               param_group['lr'] = 0.00001


        print('Accuracy of the network on the 10000 test images: %.2f %%' % (
            100 * correct / total))
        f = open("BNN.txt", 'a')
        f.write('%.2f\n' % (100 * correct / total))
        f.close()

        save_checkpoint({
        'epoch': epoch + 1,
        'state_dict': net.state_dict(),
        'optimizer' : optimizer.state_dict(),
        })

print('Finished Training')

#===============================================================================================
#                                        Weight 초기화
#===============================================================================================        

def reverse_mask(x):
    x = (x & 0xF0) >> 4 | (x & 0x0F) << 4;
    x = (x & 0xCC) >> 2 | (x & 0x33) << 2;
    x = (x & 0xAA) >> 1 | (x & 0x55) << 1;
    return x

def weight_dump(layer):
    global f_weight, fW1, fc_layer_num, conv_layer_num

    if (type(layer) == nn.Conv2d) :

        # if (conv_layer_num == 0) | (conv_layer_num == LAYER_NUM-1):
        bits = layer.weight.data.transpose(1,2).contiguous().transpose(2,3).contiguous()
        print(bits.size())
        bits = bits.cpu().float().numpy()
        for element in bits:
            # print(element)
            f_weight.write(element)
        # else:

        #     bits = layer.weight.data.transpose(0,1).contiguous()
        #     bits = bits.transpose(0,1).contiguous()
        #     bits = bits.transpose(1,2).contiguous().transpose(2,3).contiguous()
        #     bits = bits.view(-1,8).ge(0).cpu().numpy()
        #     bits = np.packbits(bits, axis=-1)

        #     for element in bits:
        #         reversed_bit = reverse_mask(element)
        #         f_weight.write(reversed_bit)

        conv_layer_num = conv_layer_num + 1


def fmap_dump_CL(x, Qbit):
    global f_fmap, layer_num
    img = x.data
    print(x.size())
    # if (layer_num == 0) | (layer_num == LAYER_NUM-1):
    print('full')
    bits = img.transpose(1,2).contiguous().transpose(2,3).contiguous()
    bits = bits.cpu().float().numpy()
    for element in bits:
        f_fmap.write(element)
    # else:
    #     print('bin')
    #     bits = img.transpose(1,2).contiguous().transpose(2,3).contiguous()
    #     bits = (bits.view(-1,8))
    #     pbits = bits.ge(0.0)
    #     pbitsn = pbits.cpu().numpy()
    #     pbitsn = np.packbits(pbitsn, axis=-1)
    #     for element in pbitsn:
    #         reversed_bit = reverse_mask(element)
    #         f_fmap.write(reversed_bit)

    layer_num= layer_num+1

def T_dump(layer):
    global N, scale_E, MM, MV, gamma, beta, num, num_of_neg_T

    if ((type(layer) == nn.BatchNorm2d) | (type(layer) == nn.BatchNorm1d)) :
        MM = layer.running_mean
        MV = layer.running_var
        gamma = layer.weight.data
        beta = layer.bias.data

        omega = gamma / (MV+layer.eps).sqrt()
        ramda = beta - omega * MM

        omega = omega.float().cpu().numpy()
        ramda = ramda.float().cpu().numpy()
        for k in omega:
            f_T1.write(k)
        for k in ramda:
            f_T2.write(k)


    if (type(layer) == BinConv2d) :
        # print(layer.conv.weight.data)
        scale_E = layer.conv.weight.data.abs().mean(1,False).mean(1,False).mean(1,False)
        # print(scale_E)
        scale_E = scale_E.float().cpu().numpy()
        for k in scale_E:
            f_T.write(k)


if args.dump:
    checkpoint = torch.load('CIFAR10_BNN_3.pth.tar')
    net.load_state_dict(checkpoint['state_dict'])
    net.eval()
    bin_op.binarizeConvParams()
    # net.apply(abs_BN)
    f_weight = open("/home/sckim/ckc/param/weight.dat", 'wb')
    net.apply(weight_dump)
    f_weight.close()

    f_fmap = open("/home/sckim/ckc/param/fmap.dat", 'wb')

    for i, (input, target) in enumerate(val_loader):
        output = (net(Variable(input)))
    f_fmap.close()

    f_T = open("/home/sckim/ckc/param/threshold.dat", 'wb')
    f_T1 = open("/home/sckim/ckc/param/threshold1.dat", 'wb')
    f_T2 = open("/home/sckim/ckc/param/threshold2.dat", 'wb')
    net.apply(T_dump)
    f_T.close()
    f_T1.close()
    f_T2.close()