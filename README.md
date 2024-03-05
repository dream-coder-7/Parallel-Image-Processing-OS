
# Parallel-Image-Processing

This C/C++ image processing project reads a PPM image, applies transformations (T1 and T2 like RGB to grayscale), and saves the result. It demonstrates parallel processing on multi-core processors: threads communicate through shared memory and synchronization using atomic operations or semaphores, and processes communicate via shared memory with synchronization semaphores or pipes.



## Tasks

<ul>

<li><b>part1.cpp</b> Image transformation: T1: Convert image to grey T2: convert image to blur-scale</li>                         
<li> <b>part2_1a</b> T1 and T2 are performed by 2 different threads of the same process. Synchronization using atomic operations  </li>
<li> <b>part2_1b</b>  T1 and T2 are performed by 2 different threads of the same process.   Synchronization using semaphores </li>
<li> <b>part2_3.cpp</b> T1 and T2 are performed by 2 different processes that communicate via
pipes. </li>
</ul>

## How to run

<b>Without Makefile</b>

Pattern: executable input_image_path output_image_path

 g++ part1.cpp -o part

./part inputs_ppm/sample_pp3_5mb.ppm output/output_part1.ppm

<b>With Makefile</b>

Just run:

make <part1/part2_1a/part2_1b/part2_3>

## Transformations:

 <b>T1</b> : RGB to Grayscale: The image that was used was RGB format and colour.
We were able to convert the image's colour to black and white through this
procedure. In order to do this, we equalize the fundamental colors of red,
green, and blue, resulting in a grayscale.
Grayscale value= (R + G + B)/3

<b>T2</b> : The Blur_convert function : performs image blurring by
averaging the color values of each pixel with its immediate neighbors. It
initializes a new vector to store the blurred image.
Then, it iterates through each pixel, excluding the borders, calculating the
average color value by summing the pixel values of the pixel and its
surrounding eight pixels.
This average value is then assigned to the corresponding pixel in the
blurred image. Finally, the original image is updated with the blurred
values. This process effectively reduces sharpness and detail, resulting in
a smoother appearance of the image.

## Results

<ul>
<li> Sequential approaches are expected to take longer than concurrency-based approaches, which are supposed to reduce time. However, this is not what is seen. The tiny size of the critical section code may lead to higher overhead in inter-process communication. </li>
<li>  We can observe that the time taken by Shared Memory and Pipes are higher
than that of the sequential approach, since they involve reading from and writing to values in
the memory.
</li>
<li> Thus, on a combined we can see that Shared Memory approach is good but it only
allows shared memory up to some limitations.</li>

<li> Pipeline takes a lot time to transfer and read on other end if data is large.
</li>

<li> Atomic Locks and Semaphore Locks perform almost same (Semaphores better a
little bit).</li>

<li> Thread-based approaches were simpler to implement due to shared data segments, making it easier to use semaphores and atomic variables. However, approaches involving multiple processes were more difficult to implement and debug due to the need to ensure correct values are passed in the correct order and received by other processes. Implementing shared memory was challenging due to the need to maintain its structure, as opposed to using pipes. </li>

## 

</ul>
