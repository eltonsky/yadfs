yadfs
=====

yet another distributed file system (YADFS) is designed after hadoop distributed file system (HFDS) but implemented in c++.

I was looking for a good project idea for learning modern c++, and also gain experience in threading and networking. 
As I am familiar with implementation of HDFS, I figure this project is a perfect fit. 

YADFS is using boost.asio for async network communication, log4cpp for logging and tinyxml for parsing configuration. It
uses gprof for call stack profiling, valgrind for memory leak checks, massif for memory profiling. In a good perspective, it 
potentially adopts google unit test framework for writng unit tests.

The class diagram and work flow are drawn using umbrello. 



NOTE: You are veeeeeery welcomed to join this project. to critisize my work, to discuss design or code, or to contribute
you ideas.


Elton
eltont9404@hotmail.com
