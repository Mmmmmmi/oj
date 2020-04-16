FROM centos:7

COPY ./docker/entrypoint /root/
COPY ./code /root/oj/

RUN yum -y install net-tools vim make gcc gcc-c++ openssl openssh-server openssh-clients \
    mysql mysql-devel \
    centos-release-scl \
    boost boost-devel boost-doc && \
    yum -y install devtoolset-7

ENTRYPOINT ["sh", "/root/entrypoint"]
