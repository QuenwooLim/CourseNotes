from builtins import object
import numpy as np

from ..layers import *
from ..fast_layers import *
from ..layer_utils import *


class ThreeLayerConvNet(object):
    """
    A three-layer convolutional network with the following architecture:

    conv - relu - 2x2 pool - affine - relu - affine - softmax

    The network operates on minibatches of data that have shape (N, C, H, W)
    consisting of N images, each with height H and width W and with C input
    channels.
    """

    def __init__(
        self,
        input_dim=(3, 32, 32),
        num_filters=32,
        filter_size=7,
        hidden_dim=100,
        num_classes=10,
        weight_scale=1e-3,
        reg=0.0,
        pooling='maxpool',
        dtype=np.float32,
    ):
        """
        Initialize a new network.

        Inputs:
        - input_dim: Tuple (C, H, W) giving size of input data
        - num_filters: Number of filters to use in the convolutional layer
        - filter_size: Width/height of filters to use in the convolutional layer
        - hidden_dim: Number of units to use in the fully-connected hidden layer
        - num_classes: Number of scores to produce from the final affine layer.
        - weight_scale: Scalar giving standard deviation for random initialization
          of weights.
        - reg: Scalar giving L2 regularization strength
        - dtype: numpy datatype to use for computation.
        - pooling: What type of pooling the network should use. Valid values are
          "maxpool" or "avgpool"
        """
        self.params = {}
        self.reg = reg
        self.dtype = dtype
        self.pooling = pooling

        ############################################################################
        # TODO: Initialize weights and biases for the three-layer convolutional    #
        # network. Weights should be initialized from a Gaussian centered at 0.0   #
        # with standard deviation equal to weight_scale; biases should be          #
        # initialized to zero. All weights and biases should be stored in the      #
        #  dictionary self.params. Store weights and biases for the convolutional  #
        # layer using the keys 'W1' and 'b1'; use keys 'W2' and 'b2' for the       #
        # weights and biases of the hidden affine layer, and keys 'W3' and 'b3'    #
        # for the weights and biases of the output affine layer.                   #
        #                                                                          #
        # IMPORTANT: For this assignment, you can assume that the padding          #
        # and stride of the first convolutional layer are chosen so that           #
        # **the width and height of the input are preserved**. Take a look at      #
        # the start of the loss() function to see how that happens.                #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        C, H, W = input_dim
        # conv layer params
        self.params["W1"] = weight_scale * np.random.randn(
          num_filters, C, filter_size, filter_size
        )
        self.params["b1"] = np.zeros(num_filters)

        # after conv with padding preserving spatial dims, then 2x2 pool halves H and W
        conv_out_h = H // 2
        conv_out_w = W // 2
        affine_input_dim = num_filters * conv_out_h * conv_out_w

        # hidden affine layer
        self.params["W2"] = weight_scale * np.random.randn(affine_input_dim, hidden_dim)
        self.params["b2"] = np.zeros(hidden_dim)

        # output affine layer
        self.params["W3"] = weight_scale * np.random.randn(hidden_dim, num_classes)
        self.params["b3"] = np.zeros(num_classes)

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        for k, v in self.params.items():
            self.params[k] = v.astype(dtype)

    def loss(self, X, y=None):
        """
        Evaluate loss and gradient for the three-layer convolutional network.

        Input / output: Same API as TwoLayerNet in fc_net.py.
        """
        W1, b1 = self.params["W1"], self.params["b1"]
        W2, b2 = self.params["W2"], self.params["b2"]
        W3, b3 = self.params["W3"], self.params["b3"]

        # pass conv_param to the forward pass for the convolutional layer
        # Padding and stride chosen to preserve the input spatial size
        filter_size = W1.shape[2]
        conv_param = {"stride": 1, "pad": (filter_size - 1) // 2}

        # pass pool_param to the forward pass for the max-pooling layer
        pool_param = {"pool_height": 2, "pool_width": 2, "stride": 2}

        scores = None
        ############################################################################
        # TODO: Implement the forward pass for the three-layer convolutional net,  #
        # computing the class scores for X and storing them in the scores          #
        # variable.                                                                #
        #                                                                          #
        # Remember you can use the functions defined in annp/fast_layers.py and  #
        # annp/layer_utils.py in your implementation (already imported).         #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        # conv - relu - pool
        if self.pooling == "maxpool":
          a, cache_conv = conv_relu_maxpool_forward(X, W1, b1, conv_param, pool_param)
        else:
          a, cache_conv = conv_relu_avgpool_forward(X, W1, b1, conv_param, pool_param)

        # affine - relu
        a_flat = a.reshape(a.shape[0], -1)
        hidden, cache_affine1 = affine_relu_forward(a_flat, W2, b2)

        # final affine
        scores, cache_affine2 = affine_forward(hidden, W3, b3)

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        if y is None:
            return scores

        loss, grads = 0, {}
        ############################################################################
        # TODO: Implement the backward pass for the three-layer convolutional net, #
        # storing the loss and gradients in the loss and grads variables. Compute  #
        # data loss using softmax, and make sure that grads[k] holds the gradients #
        # for self.params[k]. Don't forget to add L2 regularization!               #
        #                                                                          #
        # NOTE: To ensure that your implementation matches ours and you pass the   #
        # automated tests, make sure that your L2 regularization includes a factor #
        # of 0.5 to simplify the expression for the gradient.                      #
        ############################################################################
        # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

        loss, dscores = softmax_loss(scores, y)
        # add regularization
        loss += 0.5 * self.reg * (
          np.sum(W1 ** 2) + np.sum(W2 ** 2) + np.sum(W3 ** 2)
        )

        # backward
        dhidden, dW3, db3 = affine_backward(dscores, cache_affine2)
        da_flat, dW2, db2 = affine_relu_backward(dhidden, cache_affine1)
        da = da_flat.reshape(a.shape)

        if self.pooling == "maxpool":
          dx, dW1, db1 = conv_relu_maxpool_backward(da, cache_conv)
        else:
          dx, dW1, db1 = conv_relu_avgpool_backward(da, cache_conv)

        grads["W3"] = dW3 + self.reg * W3
        grads["b3"] = db3
        grads["W2"] = dW2 + self.reg * W2
        grads["b2"] = db2
        grads["W1"] = dW1 + self.reg * W1
        grads["b1"] = db1

        # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
        ############################################################################
        #                             END OF YOUR CODE                             #
        ############################################################################

        return loss, grads


class DeepConvNet(object):
  """
  A deeper convolutional network for CIFAR-10.

  Architecture:
  conv - spatial batch norm - relu - 2x2 pool -
  conv - spatial batch norm - relu - 2x2 pool -
  affine - batch norm - relu - dropout - affine - softmax

  The batch normalization steps and dropout are optional, but enabled by
  default to improve optimization on CIFAR-10.
  """

  def __init__(
    self,
    input_dim=(3, 32, 32),
    num_filters1=32,
    filter_size1=3,
    num_filters2=64,
    filter_size2=3,
    hidden_dim=500,
    num_classes=10,
    dropout=0.5,
    weight_scale=1e-3,
    reg=0.0,
    pooling='maxpool',
    use_batchnorm=True,
    dtype=np.float32,
  ):
    self.params = {}
    self.reg = reg
    self.dtype = dtype
    self.pooling = pooling
    self.use_batchnorm = use_batchnorm
    self.use_dropout = dropout != 1

    C, H, W = input_dim
    pad1 = (filter_size1 - 1) // 2
    pad2 = (filter_size2 - 1) // 2

    pooled_h = H // 4
    pooled_w = W // 4
    affine_input_dim = num_filters2 * pooled_h * pooled_w

    self.params['W1'] = weight_scale * np.random.randn(num_filters1, C, filter_size1, filter_size1)
    self.params['b1'] = np.zeros(num_filters1)
    self.params['W2'] = weight_scale * np.random.randn(num_filters2, num_filters1, filter_size2, filter_size2)
    self.params['b2'] = np.zeros(num_filters2)
    self.params['W3'] = weight_scale * np.random.randn(affine_input_dim, hidden_dim)
    self.params['b3'] = np.zeros(hidden_dim)
    self.params['W4'] = weight_scale * np.random.randn(hidden_dim, num_classes)
    self.params['b4'] = np.zeros(num_classes)

    if self.use_batchnorm:
      self.params['gamma1'] = np.ones(num_filters1)
      self.params['beta1'] = np.zeros(num_filters1)
      self.params['gamma2'] = np.ones(num_filters2)
      self.params['beta2'] = np.zeros(num_filters2)
      self.params['gamma3'] = np.ones(hidden_dim)
      self.params['beta3'] = np.zeros(hidden_dim)

    self.dropout_param = {}
    if self.use_dropout:
      self.dropout_param = {'mode': 'train', 'p': dropout}

    self.bn_params = []
    if self.use_batchnorm:
      self.bn_params = [
        {'mode': 'train'},
        {'mode': 'train'},
        {'mode': 'train'},
      ]

    for k, v in self.params.items():
      self.params[k] = v.astype(dtype)

    self.conv_param1 = {'stride': 1, 'pad': pad1}
    self.conv_param2 = {'stride': 1, 'pad': pad2}
    self.pool_param = {'pool_height': 2, 'pool_width': 2, 'stride': 2}

  def loss(self, X, y=None):
    W1, b1 = self.params['W1'], self.params['b1']
    W2, b2 = self.params['W2'], self.params['b2']
    W3, b3 = self.params['W3'], self.params['b3']
    W4, b4 = self.params['W4'], self.params['b4']

    mode = 'test' if y is None else 'train'
    if self.use_dropout:
      self.dropout_param['mode'] = mode
    if self.use_batchnorm:
      for bn_param in self.bn_params:
        bn_param['mode'] = mode

    if self.pooling == 'maxpool':
      pool_forward = max_pool_forward_fast
      pool_backward = max_pool_backward_fast
    else:
      pool_forward = avg_pool_forward_fast
      pool_backward = avg_pool_backward_fast

    a1, cache_conv1 = conv_forward_fast(X, W1, b1, self.conv_param1)
    if self.use_batchnorm:
      a1, cache_bn1 = spatial_batchnorm_forward(a1, self.params['gamma1'], self.params['beta1'], self.bn_params[0])
    else:
      cache_bn1 = None
    r1, cache_relu1 = relu_forward(a1)
    p1, cache_pool1 = pool_forward(r1, self.pool_param)

    a2, cache_conv2 = conv_forward_fast(p1, W2, b2, self.conv_param2)
    if self.use_batchnorm:
      a2, cache_bn2 = spatial_batchnorm_forward(a2, self.params['gamma2'], self.params['beta2'], self.bn_params[1])
    else:
      cache_bn2 = None
    r2, cache_relu2 = relu_forward(a2)
    p2, cache_pool2 = pool_forward(r2, self.pool_param)

    flat = p2.reshape(p2.shape[0], -1)
    hidden, cache_affine1 = affine_forward(flat, W3, b3)
    if self.use_batchnorm:
      hidden, cache_bn3 = batchnorm_forward(hidden, self.params['gamma3'], self.params['beta3'], self.bn_params[2])
    else:
      cache_bn3 = None
    hidden, cache_relu3 = relu_forward(hidden)
    if self.use_dropout:
      hidden, cache_dropout = dropout_forward(hidden, self.dropout_param)
    else:
      cache_dropout = None

    scores, cache_affine2 = affine_forward(hidden, W4, b4)

    if y is None:
      return scores

    loss, grads = 0, {}
    loss, dscores = softmax_loss(scores, y)
    loss += 0.5 * self.reg * (
      np.sum(W1 ** 2) + np.sum(W2 ** 2) + np.sum(W3 ** 2) + np.sum(W4 ** 2)
    )

    dhidden, dW4, db4 = affine_backward(dscores, cache_affine2)
    if self.use_dropout:
      dhidden = dropout_backward(dhidden, cache_dropout)
    dhidden = relu_backward(dhidden, cache_relu3)
    if self.use_batchnorm:
      dhidden, dgamma3, dbeta3 = batchnorm_backward(dhidden, cache_bn3)
    else:
      dgamma3 = dbeta3 = None

    dp2, dW3, db3 = affine_backward(dhidden, cache_affine1)
    dp2 = dp2.reshape(p2.shape)
    dr2 = pool_backward(dp2, cache_pool2)
    da2 = relu_backward(dr2, cache_relu2)
    if self.use_batchnorm:
      da2, dgamma2, dbeta2 = spatial_batchnorm_backward(da2, cache_bn2)
    else:
      dgamma2 = dbeta2 = None
    dp1, dW2, db2 = conv_backward_fast(da2, cache_conv2)

    dr1 = pool_backward(dp1, cache_pool1)
    da1 = relu_backward(dr1, cache_relu1)
    if self.use_batchnorm:
      da1, dgamma1, dbeta1 = spatial_batchnorm_backward(da1, cache_bn1)
    else:
      dgamma1 = dbeta1 = None
    dX, dW1, db1 = conv_backward_fast(da1, cache_conv1)

    grads['W4'] = dW4 + self.reg * W4
    grads['b4'] = db4
    grads['W3'] = dW3 + self.reg * W3
    grads['b3'] = db3
    grads['W2'] = dW2 + self.reg * W2
    grads['b2'] = db2
    grads['W1'] = dW1 + self.reg * W1
    grads['b1'] = db1

    if self.use_batchnorm:
      grads['gamma3'] = dgamma3
      grads['beta3'] = dbeta3
      grads['gamma2'] = dgamma2
      grads['beta2'] = dbeta2
      grads['gamma1'] = dgamma1
      grads['beta1'] = dbeta1

    return loss, grads
