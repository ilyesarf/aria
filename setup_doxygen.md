# Setting up Doxygen Documentation

Follow these steps to complete the setup of Doxygen documentation for the Fantasy Forest game:

## Install Doxygen

```bash
sudo apt install doxygen
```

## Generate Doxyfile

The Doxyfile has already been created in the project root. If needed, you can generate a new one with:

```bash
doxygen -g Doxyfile
```

## Generate Documentation

Generate the documentation by running:

```bash
make docs
```

or directly with:

```bash
doxygen Doxyfile
```

## View Documentation

After generation, view the documentation by opening:
```
docs/html/index.html
```

## Customizing Documentation

If you want to customize the documentation further, edit the Doxyfile. Some common options:

- `PROJECT_NAME`: Change the project name
- `OUTPUT_DIRECTORY`: Change the output directory
- `EXTRACT_PRIVATE`: Set to YES to include private members
- `HAVE_DOT`: Set to YES to enable diagrams (requires Graphviz)
- `GENERATE_LATEX`: Set to NO to disable LaTeX output

## Maintaining Documentation

- When adding new code, include Doxygen-style comments
- For functions: `/**` comments before the function with `@param`, `@return`, etc.
- For structs/typedefs: `/**` comments before the declaration
- For files: `/**` comments at the top of the file

## Example Comment Format

```c
/**
 * @brief Brief description
 *
 * Detailed description if needed
 *
 * @param paramName Description of parameter
 * @return Description of return value
 */
``` 