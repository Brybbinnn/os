import numpy as np
import matplotlib.pyplot as plt

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
            components = line.strip().split()  # Split the line into parts
            if len(components) < 3:  # Skip lines with insufficient data
                continue

            access_type, v_address, _ = components # Extract the type of access and virtual address along with a throwaway variable

            if access_type in ["I", "L", "S", "M"]: # We ignore the size of the access _
                page_number = int(v_address, 16) // page_size # Convert the virtual address to a page number
                page_access_list.append(page_number) # Add the page number to the access list

                if access_type == "I": # If the access type is "I" (instructions), add the page number to the instruction page set
                    instruction_page_set.add(page_number)

    return page_access_list, instruction_page_set


def plot_memory_access(page_access_list, png_file=None, instruction_page_set=None):

# You need to transform the page access list into a 2D array.
# For the X axis, use equal-sized bins into which you group subsets of the page access
# list. Example: page access list has 1M elements, you use 1000 bins, so each bin
# corresponds to 1000 elements of the page access list
# For the Y axis, you first "normalize" the page numbers (these are not contiguous, but
# it is easier to plot if you have a contiguous numbering). Define an order-preserving
# mapping from the virtual page numbers to natural numbers in the range 0 to (num-
# ber of different pages - 1). Example: If your page access list contains (only) the page
# numbers 598, 4, and 42, then you map 4 to 0, 42 to 1, 598 to 2.
# You will probably have something in the order of a few hundred to a few thousand
# different pages. The range of your Y axis (and thus this dimension of the 2D array)
# will equal the number of pages you have.
# 5
# For each bin i (X axis), and for each page k contained in the per-bin subset of the
# page access list, you set your array[i][k] to 1 (all other array elements are 0)
# You plot the result with np.imshow (you need transpose the array). Arrange your
# plot such that page 0 is on the bottom, not on the top.
# Show your output on the screen by default, or write it to a png file if an optional
# parameter pngfile is passed to your function.
# A good plot has clearly and accurately labeled title and axes.

    # TODO: Implement (remove this comment before submission if you implemented somthing)

    return


def export_page_trace(page_access_list, output_file):

    # TODO: Implement (remove this comment before submission if you implemented somthing)

    return
