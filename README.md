# Ubuntu 20.04, Lean Prover 3 Community, mathlib

Use this project to configure and build the
physvm Docker image needed by physvm_develop,
which in turn serves as a templace for a new
project building on our math-physics library.

To use this project, clone it to your local
machine, edit Dockerfile, and then build and
push to Dockerhub a new image using commands
explained below.

## Build image from Dockerfile

To build a new version of the physvm image from
the Dockerfile in this project, run the following 
command in a terminal with this directory as the
current working directory. 

``` sh
docker build -t kevinsullivan/physvm:latest . -m 8g
```

## Push image to DockerHub

To push a copy of this image to dockerhub, do this:

``` sh
docker push kevinsullivan/physvm
```

## Pull image from DockerHub

To pull a copy of the image to your local host machine, run:

```sh
docker pull kevinsullivan/physvm
```

## Start container

To launch a container using this image run the following command.
Replace %source_directory_on_host% with the host directory you want 
the VM to access as /dm. Replace %container_name% with the name you'd
like to give to the launched docker container. We suggest giving it 
a name that reflects the local directory that is mounted on its
container-local directory, /dm.

``` sh
docker run -it --cap-add=SYS_PTRACE --rm --security-opt seccomp=unconfined \
    --name %container_name% -v %source_directory_on_host%:/dm kevinsullivan/physvm \
    /bin/bash
```

## Get terminal into container

To connect to a terminal shell into the VM, do this:

``` sh
docker exec -it lean /bin/bash
```

## Stop running container

To stop a container from a terminal on your host machine, do this:

``` sh
docker stop lean
```

To stop a running image from a terminal into the container, exit the terminal process:

``` sh
exit
```
