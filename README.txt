--FOREWORD--

A "four-color problem" is about coloring bordering areas (for example, map) with only four colors, every bordering sections use different colors.
The main task of this program is to color the countries (areas) of a map created by the user with a minimum number of colors. 
Before coloring, user connects draws areas of the country and connects these sections using a graph.
This problem has existed in the world for a long time, but so far no proof has been invented for it. With all this, it is easily applicable.

--ALGORITHM--

To solve this problem, I came up with my own map coloring algorithm, which will be universal for every number of areas. 
I drew the map using the Polyline function and painted it with Polygon function (both of them are from the wingdi.h header, I just included windows.h). 
For storing the coordinates of countries I used the POINT data type.
The program randomly selects any state, paints it with the first free color and then considers the neighbors of this state. 
All Utah's neighbors are not painted over, which means that painting them will not be difficult. 
The program moves the pointer to first neighbor (each country/state has an alphabetical list of neighbor pointers), and checks for filled neighbors, 
excluding the colors used by the neighbors from the list of free colors.
After painting all the neighbors, the program remembers the pointers to them - on the last painted ones. 
The next cycle of coloring starts with them - the neighbors of the last ones painted are painted. 
All this will continue until the counter of filled ones is equal to the number of all countries.
Also, to achieve the best result, I implemented a goto function, which starts the program again if it exceeds the number of colors used (maximum - four).
