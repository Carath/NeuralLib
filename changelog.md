NeuralLib v1.4
--------------

- Fixed an array overflow in the backpropagation() function (case QUADRATIC loss without Softmax).
- Switch in the core lib from glibc rand() to PCG32.


NeuralLib v1.3
--------------

- Some dehardcoding, making the freeing functions more secure.
- Various cleanups. Adding the 'findGreaterValuesIndex' function.


NeuralLib v1.2
--------------

- Added some error handling to the core library, for increased robustness.
- Updating the animation part to SDLA v1.5.
- Improving the animation preprocessing.
