Q1
    compile- g++ 2025201069_A1_Q1.cpp `pkg-config --cflags --libs opencv4`
    run- ./a.out
    note: the pictures directory should be in the same directory as Q1.
    APPROACH:
        step1: create an energy matrix based on the approach mentioned in https://www.cs.princeton.edu/courses/archive/fall17/cos226/assignments/seam/index.html.
        step2: To find a vertical seam with lowest energy, maintain a dp matrix which stores the lowest energy required from top row to get to the current pixel.
        step3: In the last row, find the pixel with lowest energy sum, and backtrack till the first row, this path is the lowest vertical energy seam.
        step4: Remove it and go to step1.
        step5: To find and remove a horizontal seam, start from first column and sum energies towards the last column, similar to as that of vertical seam.


Q2
    compile- g++ 2025201069_A1_Q2.cpp
    run- ./a.out
    APPROACH:
        -create a trie and insert all the words into the trie.
        -flag1: start at root trie node and try to find the word, if word is found, print(1), else print(0)
        -flag2: same as flag1 but when we find the word, we find the extensions to that word and print those.
        -flag3: find the words with the levenshtein distance of 3 and print them.


Q3:
    compile- g++ 2025201069_A1_Q3.cpp
    run- ./a.out
    APPROACH:
        -maintain a map.
        -when there is update query, split it into (x, h, 0) and (y, -h, 1) and insert those into the map. The last element 0 represents start of the building and 1 represents end of the buidling.
        - when there is a display query, maintain a height priority queue and insert 0 to it because the hieght of groud is zero and max_height as zero.
            -Iterate through the map, when the element is a start element push its height into the priority queue and check if the max height in the priority queue has changed, if it is changed, then print the x coordinate and its height, if its not, then continue.
            -When the element is an end element, remove its height from the priority queue, check if the max height is same as before or not, if its not, print the current coorinate and the xurrent max_height from the priority queue.
        

