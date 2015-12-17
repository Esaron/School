EECS 425 Project 1 - HTTP Proxy
----
2015/10/31

I received permission from Professor --- to package
this in a more sane way to make everything easier. I have
included a jar and the compiled classes in the provided
archive in case you don't want to have to build them yourself
or cannot download Apache Ant onto the testing server.

NOTE: There seem to be some issues with IE revealed by some
      last minute testing (I use linux and OS X, so I did not
      see these issues initially). I did not have time to
      address these, so please use chrome or firefox to test.

Instructions:
  1) Extracting:
      - Move "jdn-425project1.tar.gz" to a location on the
        filesystem
      - Run "tar xzf jdn-425project1.tar.gz"
  2) Building:
      - If you would like to build the source code yourself,
        you will need to download Apache Ant from
        https://ant.apache.org/bindownload.cgi
      - You'll need to make sure the ant executable is on the
        system path or reference it directly if it isn't
      - In the "425-project1" directory, type "ant" to build
        the jar "proxy.jar" in the "425-project1/dist" directory
  3) Running:
      - In the "425-project1" directory, run "java -jar
        dist/proxy.jar <port>"
  4) Using:
      - Configure the proxy settings
      - Browse HTTP sites
