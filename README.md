# Simple Feed-Forward Neural Network in C  

## Description  

This project provides a foundational implementation of a feed-forward neural network using the C programming language and principles of graph theory. The primary objective is to gain a deeper understanding of how such models operate by constructing one from the ground up.  

The [Documentation.odt](Documentation.odt) file serves as a comprehensive guide to the program’s internal mechanics. Since the project's main purpose is educational, the documentation includes detailed commentary on the design decisions made throughout the development process.  


## Build

To build the project, ensure you have [GNU Make](https://www.gnu.org/software/make/) and GCC installed on your system. Follow the steps below:

1. **Clone the Repository**

   ```bash
   git clone <repository_url>
   cd <repository_directory>
   ```

2. **Compile the Project**

   Use the `make` command to compile the source files and generate the executable:

   ```bash
   make
   ```

   This will produce the executable named `main.a.out`.

3. **Additional Makefile Commands**

   - **Clean Build Artifacts**

     To remove the compiled object files and the executable, use:

     ```bash
     make clean
     ```

4. **Adding New Source or Header Files**

   If you add new `.c` or `.h` files to the project, you must update the `Makefile` accordingly. This involves:

   - **Updating Source Files**

     Add the new `.c` files to the `SRC` variables. For example, if you add `new_module.c`, include it as follows:

     ```makefile
     SRC_NEW_MODULE = new_module.c
     ```

   - **Updating Object Files**

     Similarly, add the corresponding `.o` files to the `OBJ` variables:

     ```makefile
     OBJ_NEW_MODULE = new_module.o
     ```

   - **Linking New Object Files**

     Ensure that the new object files are included in the linking step for the target executable:

     ```makefile
     $(TARGET): $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_MAIN) $(OBJ_NODE) $(OBJ_NEW_MODULE)
     	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_MATRIX) $(OBJ_MODEL) $(OBJ_MAIN) $(OBJ_NODE) $(OBJ_NEW_MODULE) -lm
     ```

   - **Compiling New Source Files**

     Add rules to compile the new source files:

     ```makefile
     new_module.o: $(SRC_NEW_MODULE) $(HEADERS)
     	$(CC) $(CFLAGS) -c $(SRC_NEW_MODULE)
     ```

   After updating the `Makefile`, run `make` again to build the project with the new files.

---

## License  

This project is licensed under the **Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)**. This permits sharing and adapting the code for non-commercial purposes, provided appropriate credit is given to the original author. For more information, refer to the [LICENSE](LICENSE) file.  
