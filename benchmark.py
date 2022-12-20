import os
import time

size = '3x3'
for input in os.listdir(f'input/samples/{size}'):
    print(f"Trying: {input}", end=" ")
    os.system(f'mkdir benchmark/{size}')
    os.system(f'touch benchmark/{size}/{input}')
    
    start = time.perf_counter()
    os.system(f'./a.out < input/samples/{size}/{input} > benchmark/{size}/{input}')
    end = time.perf_counter()
    
    print(end-start)
    path = f'benchmark/{size}/{input}'
    with open(path , 'a') as f:
        f.write(str(end-start)+'\n')
    
