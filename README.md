Symptogen
=========

Symptogen Game by H. Zaversnik, C. Champetier, E. Prana, T. Guigon, J. TA, B. Laby and C. Lejeune

Installation Guide
------------------
This installation procedure assume that the IndieLib library is already installed and built on your computer, and that you have cloned this repository.
**Caution : NEVER EVER change the CMakeLists.txt file**

### For Linux 
1. Go to the Symptogene folder and start the gui interface of Cmake. **Don't edit directly the CMakeLists.txt on you own, it could break down portability of the project**.

    <pre><code>cd Symptogen/
    cmake-gui
    </code></pre>
    
2. Select the source code folder, it is the Symptogen folder itself, and the build folder. Example for me : 
    <pre><code>/home/cecilia/Documents/Symptogen
    /home/cecilia/Documents/Symptogen/build
    </code></pre>

3. Then click on the add entry button and fill in the dialog like this :
   + name : INDIELIB_INCLUDE_DIR
   + type : PATH
   + value : the directory where the include files of the IndieLib library are located. Example for me : 
  
    <pre><code>/home/cecilia/Documents/indielib-crossplatform/IndieLib/common/include
    </code></pre>

4. Click on the add entry button and fill in the dialog like this :
   + name : INDIELIB_DEPENDENCIES_DIR
   + type : PATH
   + value : the directory where the dependencies of the IndieLib library are located. Example for me : 
  
    <pre><code>/home/cecilia/Documents/indielib-crossplatform/IndieLib/common/dependencies
    </code></pre>

5. Click on the add entry button and fill in the dialog like this :
   + name : INDIELIB_LIBRARY
   + type : FILEPATH
   + value : the static library created when you build the IndieLib package. 
  
  > The file name is libIndieLib.a and its located in a libs/ folder that can be in two places following your installation : at the root of IndieLib package or in the linux/ folder. If the libs/ folder is an hidden one, modify it's name by removing the point to make it normal folder.

  Example for me : 
  
    <pre><code>/home/cecilia/Documents/indielib-crossplatform/IndieLib/linux/libs/libIndieLib.a
    </code></pre>

6. Now you are okay to configure ! So click the Configure button. In the dialog let the default options : Unix Makefile and Native compilers. If there are no errors you can click Generate and close the cmake-gui when it's done.

7. Now you can compile and run the executable : 
    <pre><code>cd build/
    make
    ./Symptogen
    </code></pre>

### For Windows
*This installation guide is made to use the Visual Studio Express 2012 edition with the last version of IndieLib SDK*

**coming soon**
