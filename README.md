# embAppStartup
This is a minimal embedded application skeleton focusing on application startup and initialization.

It illustrated two ways to do scatter loading, which is the most important step for application startup and initialization:
* Call ARM Library's startup code
  ARM Library's entry point is __main, which does scatter loading first, then call __rt_entry. After basic setup in the reset_handler, where it is appropriate to do scatter loading, make your code jump to __main:
  ```asm
  B __main
  ```
  Depending on what your application does, you may not need all other statrup implimentation done by __rt_entry. To override the __rt_entry from ARM Library, define your own __rt_entry, for example:
  ```asm
  //Override ARM library's __rt_entry:
      .global __rt_entry
      .type   __rt_entry @function
    __rt_entry:
    B  appmain
    ```
    which jumps to your application's main entry appmain directly.
    This is the recommended way for application startup and initialiation because of its robustness. No matter what kind of non-root regions you add and how many, ARM linker will automatically create a region table in the root region. The ARM Library function __main calls __scatterload, which goes through the region table and initialize the various execution-time regions:
    
    * Initialize ZI regions to zero
    * Copy RW data regions
    * Copy non-root RO regions

* Ceate your own startup code for scatter loading
  You can also create your own scatter loading function if you don't want to depend on ARM Library. For each ZI region or non-root region, you need to add a section of initialization code. This is implemented by macros in this example project. If you add a new non-root region, do not forget to add its initialization in my__main (it's named so indicating it is the counterpart of __main) function. This is a drawback if this approach. ARM linker won't create a region table for you. 
 
 # Build and run
 the armclang toolchain is used to build this example. And the build script is CMake script. It's tested on ARM FVP FVP_Base_RevC-2xAEMv8A. Assuming your development environment is correctly set up, this is the steps to build:
 1. Creae a build directory: mkdir build
 2. CD to build: cd build.
 3. cmake ../ -DUSE_ARM_LIB_STARTUP=0
 4. cmake --build . -v
 
 Hopefully, you'll get appStartupDemo.axf up to this point.
 The build option USE_ARM_LIB_STARTUP allows you to switch between the two initialization methods. 
 
 To run the FVP, use these model parameters:
 
 -C bp.secure_memory=false -C bp.terminal_0.mode=raw -C bp.refcounter.non_arch_start_at_default=1 
