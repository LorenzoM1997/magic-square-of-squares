# Magic Square of Squares
Algorithm to search magic squares of squares

## History
In 1770 Leonhard Euler was the first to construct 4×4 magic squares of squares, as mentioned above. But nobody has yet succeeded in building a 3×3 magic square of squares or proving that it is impossible. Edouard Lucas worked on the subject in 1876. Then, in 1996, Martin Gardner offered $100 to the first person who could build one. Since this problem — despite its very simple appearance — is incredibly difficult to solve with nine distinct squared integer, there is an ongoing challenge online to find a magic square with 7, 8, or 9 square numbers. There is only one solution found for 7.

## The method
To solve this problem I created a search algorithm which is trying to identify sequences of 7 or more squares. To find such square I attempt to identify square numbers in groups of two or three, where each of the two or three must have the same distance. For example, i fix a *n* and the algorithm will print all the groups of two or three which have a different of *n* between the members. After we select those groups that have at least one group of three and two groups of two, we analyze them to see if it can be composed in a magic square.
To be composed in a magic square, we compare the distance of the last element of every group. Taken three groups (of which one has three members), the last element of each group must be either:
- equally spaced or
- equally spaced - distance of exactly *n*.
We check then for these conditions and if one is qualified with such condition we print *** to screen near the number.

## Running the program
You are free to run the program and try yourself to be the first one to find the square. If you use the code and find such magic square, you can keep the win but please refer me as contributor! Thanks.
