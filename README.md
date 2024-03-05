# Parallel-Image-Processing-OS
This C/C++ image processing project reads a PPM image, applies transformations (T1 and T2 like RGB to grayscale), and saves the result. It demonstrates parallel processing on multi-core processors: threads communicate through shared memory with atomic operations or semaphores, and processes communicate via shared memory with semaphores or pipes.
