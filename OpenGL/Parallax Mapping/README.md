# 法线贴图

## 坐标系转换问题

### TBN矩阵

TBN矩阵解决了
将发现贴图u-v坐标系 映射到 模型坐标系

### TBN坐标系转化世界坐标系

Texture u-v坐标系 映射到 世界坐标系方法

T = mat3(world) * tangent

B = mat3(world) * bitangent

N = mat3(world) * normal

### TBN矩阵怎么构建

TBN 矩阵

(T,

 B,

 N)

world = TBN * (u,v,n)

TBN是切线空间基向量在世界空间一个映射，T * u是局部向量在世界空间的映射

w_x = T * u , B * u, N * u

w_y = T * v , B * v, N * v

w_z = T * n , B * n, N * n

