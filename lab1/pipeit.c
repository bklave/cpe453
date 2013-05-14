#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
	pid_t childpid1, childpid2;
	int fd[2];
	int outfileStream;

	// Create a pipe for interprocess communication.
	if (pipe(fd)) {
		perror("Pipe failed.\n");
		exit(-1);
	}

	// Fork a process
	if ((childpid1 = fork()) == -1) {
		perror("Fork 1 failed.\n");
		exit(-1);
	}

	// First child process.
	if (childpid1 == 0) {

		// Overwrite "stdout" with the "write" end of my pipe.
		dup2(fd[1], 1);

		// Clean up the pipe
		close(fd[0]);
		close(fd[1]);

		// Execute the "ls" program.
		execlp("ls", "ls", 0);

		// If the child process gets here, it's an error
		perror("execl didn't work");
		exit(-1);
	} else {

		// Fork again
		if ((childpid2 = fork()) == -1) {
			perror("Fork 2 failed.\n");
			exit(-1);
		}

		// Second child process.
		if (childpid2 == 0) {

			// TODO: Error check
			outfileStream = open("outfile.txt", O_WRONLY | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

			// Replace "stdin" with the "read" end of my pipe.
			dup2(fd[0], 0);

			// Replace "stdout" with the file I opened up
			dup2(outfileStream, 1);

			// Clean up file streams.
			close(fd[0]);
			close(fd[1]);
			close(outfileStream);

			// TODO: Error check
			execlp("sort", "sort", "-r", 0);

			// If the child process gets here, it's an error
			perror("sort didn't work");
			exit(-1);
		} else {
			// Parent process.
			int status;

			// TODO: Parent needs to close its file descriptors, and also wait for its children.
			close(fd[0]);
			close(fd[1]);

			if (-1 == wait(&status)) {
				perror("wait error");
				exit(-1);
			}
			if (-1 == wait(&status)) {
				perror("wait error");
				exit(-1);
			}

			// End the thread
			exit(0);
		}
	}

}
