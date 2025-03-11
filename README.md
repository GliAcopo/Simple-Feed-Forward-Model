# Graph-Based Feed-Forward Neural Network Library in C

# Work In Progress
This project is still a work in progress, many things will be subject to change in the near future.

## Overview

This repository hosts a library for constructing, training, evaluating, and managing feed-forward neural network models, all implemented in C. This project leverages graph theory and adjacency matrices to represent the network's structure, providing a more "theoretical" perspective on neural network design.

## Key Features

- **Model Construction:** Define neural networks as graphs using nodes and weighted edges, represented through adjacency matrices.
- **Training Module:** Implement training routines to adjust weights and optimize network performance.
- **Inference Engine:** Compute outputs from the network trough a given prompt.
- **Memory Management:** Create and delete models to maintain optimal resource usage.
- **Demonstrative Example:** Train the network to compute the square root of an integer (provided in binary form), showcasing a practical application of the library.

## Educational Focus

The primary goal of this project is to deepen understanding of feed-forward neural networks by building one from the ground up. Detailed design decisions, implementation insights, and theoretical underpinnings are documented in [Documentation.odt](Documentation.odt).

## Testing

The tests I've done during the development are integrated within the `Tests` folder, where edge cases and incorrect inputs are validated, ensuring the reliability of the library.

---

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
