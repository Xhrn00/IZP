# README

## Cluster Analysis: 2D Nearest Neighbor - Single Linkage

This project implements a simple clustering algorithm using the **2D nearest neighbor (single linkage) method**. The program reads objects with their **x, y coordinates** from a file, clusters them, and prints the final clusters to the console.

---

## Compilation and Execution

### Compilation:
To compile the program, use:
```sh
gcc -std=c99 -Wall -Wextra -o cluster cluster.c -lm
```

### Execution:
The program takes one or two command-line arguments:
```sh
./cluster input_file.txt [N]
```
- `input_file.txt`: The file containing objects to be clustered.
- `[N]` (optional): The desired number of final clusters (default: 1).

---

## Input File Format

The input file should follow this structure:
```
count=5
1 250.0 500.0
2 100.0 100.0
3 500.0 100.0
4 600.0 600.0
5 300.0 200.0
```
- The first line specifies the **number of objects**.
- Each subsequent line represents an **object** with its `ID x y` coordinates.
- IDs must be unique.
- x and y coordinates should be in the range `[0, 1000]`.

---

## Program Flow
1. Reads objects from the input file.
2. Creates a cluster for each object.
3. Merges clusters using the **single linkage method** until the desired number of clusters is reached.
4. Prints the final clusters in the format:
```
Clusters:
cluster 0: 1[250,500] 5[300,200]
cluster 1: 2[100,100] 3[500,100] 4[600,600]
```

---

## Function Descriptions

### **Cluster Management**
- `init_cluster(struct cluster_t *c, int cap)`: Initializes a cluster with a given capacity.
- `clear_cluster(struct cluster_t *c)`: Clears a cluster and frees memory.
- `resize_cluster(struct cluster_t *c, int new_cap)`: Expands a cluster's capacity.
- `append_cluster(struct cluster_t *c, struct obj_t obj)`: Adds an object to a cluster.
- `merge_clusters(struct cluster_t *c1, struct cluster_t *c2)`: Merges two clusters and sorts them.

### **Distance Calculation**
- `obj_distance(struct obj_t *o1, struct obj_t *o2)`: Computes Euclidean distance between two objects.
- `cluster_distance(struct cluster_t *c1, struct cluster_t *c2)`: Finds the minimum distance between two clusters.
- `find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)`: Finds the two closest clusters.

### **Cluster Processing**
- `remove_cluster(struct cluster_t *carr, int narr, int idx)`: Removes a cluster from the array.
- `load_clusters(char *filename, struct cluster_t **arr)`: Reads clusters from a file.
- `new_clusters_size(struct cluster_t *clusters, int size, int new_size)`: Merges clusters until the target count is reached.
- `print_clusters(struct cluster_t *carr, int narr)`: Prints the final clusters.

---

## Error Handling
The program handles the following errors:
- Invalid file format.
- Incorrect number of arguments.
- Non-numeric or negative values for cluster count.
- Objects with out-of-range coordinates.
- More clusters requested than available objects.

---

## Example Usage
### Input File (`input.txt`)
```
count=4
1 200.0 300.0
2 400.0 500.0
3 100.0 600.0
4 700.0 800.0
```

### Command Execution
```sh
./cluster input.txt 2
```

### Output
```
Clusters:
cluster 0: 1[200,300] 3[100,600]
cluster 1: 2[400,500] 4[700,800]
```

---

## Author
**Hryn Yaroslav**

