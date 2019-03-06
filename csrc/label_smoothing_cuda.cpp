#include <torch/torch.h>

#include <vector>

// CUDA forward declarations

std::vector<at::Tensor> softmax_xentropy_cuda(
    const at::Tensor &input,
    const at::Tensor &labels,
    const float smoothing,
    const bool half_to_float);

at::Tensor softmax_xentropy_backward_cuda(
    const at::Tensor &grad_loss,
    const at::Tensor &logprobs,
    const at::Tensor &labels,
    const float smoothing,
    const bool half_to_float);

// C++ interface

#define CHECK_CUDA(x) AT_ASSERTM(x.type().is_cuda(), #x " must be a CUDA tensor")
#define CHECK_CONTIGUOUS(x) AT_ASSERTM(x.is_contiguous(), #x " must be contiguous")
#define CHECK_INPUT(x) CHECK_CUDA(x); CHECK_CONTIGUOUS(x)

std::vector<at::Tensor> softmax_xentropy_forward(
    const at::Tensor &input,
    const at::Tensor &labels,
    const float smoothing,
    const bool half_to_float) {
    CHECK_INPUT(input);
    CHECK_INPUT(labels);

    return softmax_xentropy_cuda(input, labels, smoothing, half_to_float);
}

at::Tensor softmax_xentropy_backward(
    const at::Tensor &grad_loss,
    const at::Tensor &logprobs,
    const at::Tensor &labels,
    const float smoothing,
    const bool half_to_float) {
    CHECK_INPUT(grad_loss);
    CHECK_INPUT(logprobs);
    CHECK_INPUT(labels);

    return softmax_xentropy_backward_cuda(grad_loss, logprobs, labels, smoothing, half_to_float);
}

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
    m.def("forward", &softmax_xentropy_forward, "Softmax cross entropy loss with label smoothing forward (CUDA)");
    m.def("backward", &softmax_xentropy_backward, "Softmax cross entropy loss with label smoothing backward (CUDA)");
}
