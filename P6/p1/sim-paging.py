import numpy as numpy
import matplotlib.pyplot as matplotlib
#!/usr/bin/env python3

#
# Put your solution into the three functions in this file
#
# valgrind --tool=lackey --trace-mem=yes ./main 2> trace-main.txt

# A (very short) sample trace is in the template. Find more traces on noskel.mooo.com in
# /home/sty24/traces. You should have at least 2 traces for testing.


def get_page_list(filename):
    # Expected functionality: Read content of file (valgrind/lackey output), and then
    # - find all lines containing memory access:
    #   Line has I, L, M, S at the beginning (first two columns), then a space
    #   After that an address in hex notation
    #   Finally, a comma and an access size in byte
    # - construct an ordered list of memory pages accessed (based on the address)
    # - construct a set of memory pages that contain instructions (based on address in 'I' lines)

    page_size = 4096  # page size for Intel x86-64 system in bytes
    page_access_list = []
    instruction_page_set = set()

    with open(filename, 'r') as file:
        for line in file:
            if "==" in line:
                continue # Skip lines that don't contain memory access

            components = line.strip().split()  # Split the line into parts
            temp = components[1]
            temp = temp.split(",")
            components[1] = temp[0]
            components.append(temp[1])

            if len(components) == 3:  # Skip lines with insufficient data
                access_type, v_address, _ = components # Extract the type of access and virtual address along with a throwaway variable

                if access_type in ["I", "L", "S", "M"]: # We ignore the size of the access _
                    page_number = int(v_address, 16) // page_size # Convert the virtual address to a page number
                    page_access_list.append(page_number) # Add the page number to the access list

                    if access_type == "I": # If the access type is "I" (instructions), add the page number to the instruction page set
                        instruction_page_set.add(page_number)

    return page_access_list, instruction_page_set


def plot_memory_access(page_access_list, png_file=None, instruction_page_set=None):

    # Get unique page numbers and sort them then map each unique page number to an index
    unique_pages = sorted(set(page_access_list))
    page_to_index = {page: i for i, page in enumerate(unique_pages)}

    # Transform the page access list into a 2D array
    num_bins = 1000
    bin_size = len(page_access_list) // num_bins
    array = numpy.zeros((num_bins, len(unique_pages)))

    # Populate the bins
    for i in range(num_bins):
        for page in page_access_list[i*bin_size:(i+1)*bin_size]:
            array[i, page_to_index[page]] = 1

    # Plot the result
    matplotlib.imshow(array.T, aspect='auto', origin='lower')
    matplotlib.xlabel('Memory Access Bins')
    matplotlib.ylabel('Page')
    matplotlib.title('Memory Access Plot')

    # Save or show the plot
    if png_file != None:
        matplotlib.savefig(png_file)
    else:
        matplotlib.show()

    return


def export_page_trace(page_access_list, output_file):

    ret_list = []

    for i in range(len(page_access_list) - 1): # If the current page access is different from the next one, we add it to the list
        if page_access_list[i] != page_access_list[i+1]:
            ret_list.append(page_access_list[i]) 

    # Add the last page access as it is not in the loop
    ret_list.append(page_access_list[-1])

    # Write the page accesses to the outputfile
    with open(output_file, 'w') as f:
        for line in ret_list:
            f.write(str(line) + '\n')


def main():
    getList, getInstructions = get_page_list("./trace-ls.txt")
    plot_memory_access(getList, None, getInstructions)
    #export_page_trace(getList, "./compile-scheduler-out.txt")

if __name__ == "__main__":
    main()

