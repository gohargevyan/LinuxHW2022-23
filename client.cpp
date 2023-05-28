#include <iostream>
#include <climits>
#include <sys/mman.h>
#include <sys/stat.h>      
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

class Func
{
public:
      int id;
      int arg_1;
      int arg_2;
      int result;
};

int main()
{
  const char* fname = "/shmem";
  int shmem_fd = shm_open(fname, O_RDWR|O_CREAT, 0666);
  if(shmem_fd == -1)
  {
    std::cerr << "Failed to open file" << "\n";
    exit(errno);
  }
  int shmem_size = sizeof(Func);
  if(ftruncate(shmem_fd, shmem_size) == -1)
  {
    std::cerr << "ftruncate id failed" << "\n";
    exit(errno);
  }

   Func* func = (Func*)mmap(nullptr, shmem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmem_fd, 0);
   if(func == MAP_FAILED)
   {
     std::cerr << "Failed to map file" << "\n";
     exit(errno);
   }
  close(shmem_fd);

  const char* sem_name_1 = "/sem_1";
  const char* sem_name_2 = "/sem_2";
  sem_t* sem_1 = sem_open(sem_name_1, O_CREAT, 0666, 0);
  sem_t* sem_2 = sem_open(sem_name_2, O_CREAT, 0666, 0);
  if(sem_1 == SEM_FAILED)
  {
    std::cerr << "Failed to open semaphore" << "\n";
    exit(errno);
  }
  if(sem_2 == SEM_FAILED)
  {
    std::cerr << "Failed to open semaphore" << "\n";
    exit(errno);
  }
  std::cout << "Enter the function number where 0 is add, 1 is sub, 2 is mul and 3 id division: ";
  std::cin >> func->id;
  if(func->id == INT_MIN)
  {
    exit(1);
  }
  std::cout << "Enter 2 arguments: ";
  std::cin >> func->arg_1 >> func->arg_2;
  sem_post(sem_1);
  sem_wait(sem_2);
  if(func->id == 3 && func->result == INT_MIN)
  {
    exit(EXIT_FAILURE);
  }
  std::cout << "The result is: " << func->result << "\n";
  sem_close(sem_1);
  sem_close(sem_2);
  munmap(func, shmem_size);

  return 0;
}

