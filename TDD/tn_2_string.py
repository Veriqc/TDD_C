import numpy as np
from TN import Index,Tensor,TensorNetwork
import csv
import subprocess

def tn_2_string(tn:TensorNetwork):
    with open('tensor.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        
        writer.writerow(["tensor.data","tensor.data.shape","tensor.index_set"])
        for ts in tn.tensors:
            data = np.array2string(ts.data,
                                   formatter={
                                       'all': lambda x: f'{x.real:.1f}{x.imag:+.1f}j' 
                                       if isinstance(x, complex) else f'{x:.1f}'}
                                   ).replace("\n", " ")
            shape = " ".join(str(i) for i in ts.data.shape)
            index_temp = [[i.key,i.idx] for i in ts.index_set]
            index_str = ""
            for i in index_temp:
                index_str = index_str + i[0] +":"+ str(i[1]) + " "
            writer.writerow([data,shape,index_str.strip()])

def order_2_strig(all_indexs):
    with open('order.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        for index in all_indexs:
            writer.writerow([index])
def run_c(qubit):
    cpp_executable = "../build/TDD_C "
    tesnosr_data = "tensor.csv "
    order_data = " order.csv"
    # function choose
    process = subprocess.Popen(cpp_executable+tesnosr_data+str(qubit)+order_data, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)

    stdout, stderr = process.communicate()