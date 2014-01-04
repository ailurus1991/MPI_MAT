# Sparse Matrix Multiplication

### Step 0

Go in the src/ directory and compile:

    make
hah 
### Step 1

Set mpd.host at the first host machine:

    node1
    node2
    node3

### Step 2

Run mpd processes on all these host machines

    mpd &

### Step 3

Run the binary:

    mpirun -n 10 ./run
