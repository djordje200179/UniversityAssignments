FROM ubuntu:20.04

RUN apt-get update
RUN apt-get upgrade -y

RUN apt-get install -y openssh-server
RUN apt-get install -y build-essential qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu gdb-multiarch g++-riscv64-linux-gnu

RUN mkdir /var/run/sshd
RUN echo 'root:Djole2001' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

EXPOSE 22

CMD ["/usr/sbin/sshd","-D"]