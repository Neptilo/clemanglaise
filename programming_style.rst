Programming style to use when editing this project
==================================================

* Name class names and their corresponding files using Camel Case. (This is not QtCreator's default!)
* Name attributes, methods, signals and slots using underscore style.
* In include lists, put in order includes of:

  * the source's header
  * C headers
  * C++ headers
  * Qt headers
  * local headers of the project

* Sort each include list by name
* Initialize pointer attributes to NULL in initialization lists. Set them to NULL after a delete (except if it is redefined right after). We should always be able to know if a pointer `p` points to an actual object with the test `if(p)`.
* Do not keep console prints (such as `qDebug()`) in a commited file set.
* Do not keep code blocks commented out in a commited file set.
