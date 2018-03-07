### 哔哩哔哩 bilibili 

* 使用nginx + lua 开发支持高并发的Web接口

    ngx_lua是Nginx的一个模块，将Lua嵌入到Nginx中，从而可以使用Lua来编写脚本，这样就可以使用Lua编写应用脚本，部署到Nginx中运行，这样就可以使用Lua语言开发高性能Web应用了。

    Web应用的业务逻辑是，对于一个访问源站的视频请求，如果这个视频被请求的次数超过了某个次数，那么这个视频是一个热点的视频，需要将这个视频分发到cdn服务商的服务器里面。

    业务逻辑如下：
        
    1. 解析url的post请求里面的参数，拿到对应请求的视频id

    2. 对于一个视频id，通过redis来记录它的请求次数，如果超过某个值就认为这个视频是一个热点视频，需要存放到cdn服务商的服务器中。

    3. 如果对于当前视频id，它需要被放到cdn服务器中，对视频id进行哈希取余操作 (如： hash(id)%1000 )，计算出将当前视频需要分配到哪一个cdn的服务商的服务器中。

    4. 如何将视频分发到cdn中？
        
        cdn服务商（以七牛云为例）提供了镜像存储的服务，如下图所示。利用镜像存储，可以将源站的视频拖到cdn服务商的服务器中。向cdn发送一个当前视频的http请求，cdn会自动的从源站把视频拖过来。
        ![](1.png) 

    5. cdn从源站拖视频的时候，需要设计一个http的请求的参数来防盗链。比如：
        ```
        http://源站ip/path?query=参数
        #参数=hash(当前时间+有效期，path，secret_key)
        ```
    
    * 难点（对于我来说）

        1. 使用在nginx + lua脚本开发web应用，需要熟悉nginx的基本原理和如何在nginx下嵌入lua脚本来开发web应用。
    
        2. 使用lua脚本开发web应用，更多的是看重lua语言效率比较高的优点，但是使用lua进行web开发并不像php一样有很多成熟的内置函数和库，比如进行md5加密的函数和发送http请求的函数，甚至是ip_to_long这样的函数，lua里面都需要自己来实现。

    * nginx+lua 的性能

        在openresty的官网文档里面，https://openresty.org/cn/benchmark.html 作者也给出解释说openresty比nginx + php的方式好。如下图所示：

        ![官网里面的解释](2.png)

        下面的是项目完成后，我用Apache AB压测的结果，如下所示：
        ```
        ab -n 50000 -c 10 -p ./post.txt -T application/x-www-form-urlencoded "http://127.0.0.1/abpost"

        This is ApacheBench, Version 2.3 <$Revision: 1706008 $>
        Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
        Licensed to The Apache Software Foundation, http://www.apache.org/

        Benchmarking 127.0.0.1 (be patient)
        Completed 5000 requests
        Completed 10000 requests
        Completed 15000 requests
        Completed 20000 requests
        Completed 25000 requests
        Completed 30000 requests
        Completed 35000 requests
        Completed 40000 requests
        Completed 45000 requests
        Completed 50000 requests
        Finished 50000 requests


        Server Software:        openresty/1.13.6.1
        Server Hostname:        127.0.0.1
        Server Port:            80

        Document Path:          /abpost
        Document Length:        175 bytes

        Concurrency Level:      10
        Time taken for tests:   4.303 seconds
        Complete requests:      50000
        Failed requests:        0
        Non-2xx responses:      50000
        Total transferred:      16550000 bytes
        Total body sent:        8800000
        HTML transferred:       8750000 bytes
        Requests per second:    11619.96 [#/sec] (mean)
        Time per request:       0.861 [ms] (mean)
        Time per request:       0.086 [ms] (mean, across all concurrent requests)
        Transfer rate:          3756.06 [Kbytes/sec] received
                                1997.18 kb/s sent
                                5753.24 kb/s total

        Connection Times (ms)
                    min  mean[+/-sd] median   max
        Connect:        0    0   0.1      0      14
        Processing:     0    1   1.5      1      65
        Waiting:        0    1   1.5      1      65
        Total:          0    1   1.5      1      65

        Percentage of the requests served within a certain time (ms)
        50%      1
        66%      1
        75%      1
        80%      1
        90%      1
        95%      1
        98%      2
        99%      3
        100%     65 (longest request)
        ```
        openresty的结果是10k/s ，比原来用php的实现方式提高了大约30%。


### 思科 cisco

* Wireshark插件

    Wireshark是一个网络数据包的分析软件，虽然已经支持数千种协议，但是对于cisco 内部使用的一些协议，Wireshark 还没有办法解析。如果想要解析自定义协议的数据包，可以使用lua编写Wireshark的插件，是其可以解析自定义协议的数据包。

* teacat 开发

    teacat 是cisco 内部对基于Python的路由器或交换机的镜像文件进行自动化测试的项目名的简称。    
    使用Python远程连接到路由器的集群上面，根据需求搭建相应的测试的网络模型，对相关的路由器使用python进行自动化的配置，最后通过网络的状态和相关路由器的状态来判断这个版本的系统镜像是否对是暴露出来的bug完全的修复。

    * 难点（对于我来说）
    
        1. 需要熟悉搭建网络环境，在cisco的路由器上配置网络协议。

        2. 熟悉teacat的业务逻辑，使用teacat来自动化的搭建一个网络，并完成相关的路由器的配置。






        

       