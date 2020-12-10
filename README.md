# TenGigaRays

A very simple C++ ray tracing render.

## Todo:

### 2018/9/10
1. Add Glass material
2. Traditional denoise

因为光线追踪的特性（Lambert项中的反射为随机）。导致了光线追踪算法不可避免的噪声。我们的目标是采用传统的滤波方法去掉一定的噪声，达到十倍光照跟踪的效果（SSAA 200X -> SSAA 2000X）。

光线跟踪算法产生的噪声理论上来说是随机噪声，假设其噪声亮度和位置平均分布，该噪声可被视为随机噪声（pepper noise）。因此我们尝试了中值滤波（median filter）和双边滤波器（Bilateral Filter）。

![Filter Test](/pics/filter_test.png)

测试发现，双边滤波器的观感更好，我们尝试了不同宽度的双边滤波器

![Bilateral Filter Width Test](/pics/Bil_filter_test.png)

目前认为width=9时整体观感最佳。

### 2018/9/11
1. Add box and light source
2. Use SRGAN

## Reference:
1. svpng
2. "Ray Tracing in One Weekend"
3. Carlo Tomasi, Roberto Manduchi, Bilateral Filtering for Gray and Color Images, proceedings of the ICCV 1998
4. Sylvain Paris, Pierre Kornprobst, Jack Tumblin, Frédo Durand, A Gentle Introduction to Bilateral Filtering and its Applications, SIGGRAPH 2008
