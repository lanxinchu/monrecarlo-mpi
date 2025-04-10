### 使用前的配置要求

1. 安装c/c++编译器gcc/g++

2. 安装make

3. 安装并配置mpich

4. 集群使用时需配置静态ip及域名解析文件并更改主机名，配置ssh免密登录，建立nfs共享文件夹

### 单机使用环境配置

下载mpich安装包，进入压缩包所在文件夹，执行如下命令解压：`tar -zxvf ./mpich-4.2.1.tar.gz`

为避免错误，在安装MPICH之前，分别执行以下命令：

保证安装的源最新 `sudo apt-get update`

安装gcc编译器，用于编译C语言 `sudo apt-get install gcc`

安装g++编译器，用于编译C++ `sudo apt-get install g++`

安装gfortran编译器，用于编译fortran语言 `sudo apt-get install gfortran`

安装make，编译源码使用 `sudo apt-get install make`

切换到 MPICH 目录，使用./configure配置 `./configure --disable-fortran` 安装到本地目录

配置完成后，应显示 “Configuration completed.”

构建和安装 MPICH `make; sudo make install`

如果构建成功，则应该可以输入`mpiexec --version`并看到mpich版本信息

用`which mpicc`和`which mpiexec`检查安装是否成功，如果有这两个可执行程序的路径显示，则表示安装成功

### 集群使用环境配置

#### 1.网络环境配置

##### 修改主机名

`$ sudo hostnamectl set-hostname xxx` xxx为主机名

重启虚拟机 $ reboot

##### 桥接网卡配置静态ip

在宿主机中打开命令行 使用 ipconfig查看当前无线网络ipv4地址及网关gateways

打开虚拟机配置，将网络适配器模式改为桥接

进入01-network-manager-all.yaml网络配置文件配置静态ip

`$ sudo nano /etc/netplan/01-network-manager-all.yaml`

按照格式修改静态ip

要求:ip地址前三个网段(192.168.1)与宿主机相同，网关与宿主机相同

`$ sudo netplan apply `使新配置生效

##### 修改host文件

`$ sudo nano /etc/hosts`添加主机名-ip映射

##### 添加用户及免密登录配置

添加用户,赋予sudo权限

`$sudo adduser xxx `#添加用户xxx

`$sudo passwd xxx` #修改用户lhr口令

`$sudo adduser xxx sudo` #添加用户lhr到sudo组

ssh免密配置

在每个虚拟机中输入以下命令

生成密钥` $ ssh-keygen`

分发密钥 `$ ssh-copy-id yyy@xxxx`

yyy为登录用户名，xxxx为主机名

#### 2. 集群分布式文件系统安装配置（nfs)

创建一个目录，以后用该目录作为共享目录 ：`mkdir /xxx`

在cloud-lzz-01节点中安装nfs `sudo apt-get install nfs-kernel-server`

设置nfs服务器，将01节点设置为nfs服务器，更改exports文件 `sudo vi /etc/exports`

在文件的最后附件上所有同步的客户端的虚拟机的ip和权限

重启nfs服务器 `sudo /etc/init.d/nfs-kernel-server restart`

nfs服务器配置完毕，接下来配置其他节点

在其他节点(02、03、04、05、06)配置nfs `sudo apt install nfs-common`

在其他节点中执行 `sudo mount -t nfs ip:/xxx /yyy`

ip为nfs服务器ip地址

/xxx 服务器共享文件夹

/yyy 为本机共享文件夹挂载目录

### 文件目录说明

filetree

├── README.md

├── MonteCarlo

├── MonteCarlo.c

└── mpi_config_file

### 程序安装

1. 将c语言源文件和mpi_config_file配置文件保存到nfs共享文件中

2. 使用mpicc编译c语言源文件生成可执行文件，并授予可执行权限

`mpicc -o MonteCarlo MonteCarlo.c && chmod -x MonreCarlo`

### 部署

1. 在mpi_config_file文件中配置节点主机名和所使用的核数，如：

`cloud-lzz-01:4 `

2. 运行可执行文件`mpiexec -n x -f mpi_config_file ./MonteCarlo y`

其中:

-n后为运行的进程数x

-f为配置文件mpi_config_file的路径

y为模拟的点数

### 作者

lanxinchu
