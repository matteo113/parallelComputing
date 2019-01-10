import os

time = '00:20:00'
threads = ['1', '2', '4', '8', '10', '12', '16', '20']
cpus = ['1', '10', '20', '40', '60', '80', '100', '120']
long = ['1', '10', '100', '1000', '10000']

for t in threads:
    os.system('sbatch --time='+time+' --cpus-per-task='+t+' --output=julia_thread_simple_'+t+'.o%j runts.sh')
    os.system('sbatch --time='+time+' --cpus-per-task='+t+' --output=julia_thread_static_'+t+'.o%j runtst.sh')
    for l in long:
        os.system('sbatch --time='+time+' --cpus-per-task='+t+' --output=julia_thread_dynamic_'+t+'_'+l+'.o%j runtd.sh '+l)

for c in cpus:
    os.system('sbatch --time='+time+' --ntasks='+c+' --output=julia_mpi_simple_'+c+'.o%j runms.sh')
    os.system('sbatch --time='+time+' --ntasks='+c+' --output=julia_mpi_static_'+c+'.o%j runmst.sh')
