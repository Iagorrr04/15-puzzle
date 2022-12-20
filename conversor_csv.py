# converter os inputs tudo pra um csv pra facilitar plotar
import os
import argparse

# só pra poder mandar o size como parâmetro.
parser = argparse.ArgumentParser()
parser.add_argument('--size', type=str, default='3x3')
size = parser.parse_args().size
path = f'benchmark/{size}'
iterations = 3
total_moves = 4
moves = 5
time = 6
with open(f'benchmark/csvs/{size}.csv', 'w') as outcsv:
    print('nome_input,total_movimentos,total_iterations,movimentos,tempo')
    outcsv.write('nome_input,total_movimentos,total_iterations,movimentos,tempo\n')

    for out in os.listdir(path):
        if not out.endswith('.csv'):
           
            with open(f'{path}/{out}', 'r') as f:
                full_out = f.read().split('\n')
                iterations_v = full_out[iterations].split()[1]

                print(f'{out}, {full_out[total_moves]}, {iterations_v}, {full_out[moves]}, {full_out[time]}')
                outcsv.write(f'{out}, {full_out[total_moves]}, {iterations_v}, {full_out[moves]}, {full_out[time]}\n')
