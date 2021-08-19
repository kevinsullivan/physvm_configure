# Ubuntu 20.04, Lean Prover 3 Community, mathlib

This directory supports building of a docker
image for using our physics library module. At
present, this builder just imports and exports
leanvm without changes. However, it builds and
pushes a new image, physvm, which is what our
physvm_develop forkable project template needs
to import. Here are the commands to buid, push
(to DockerHub), and use our physvm containers.
We assume you've already got docker running on
your computer. Be logged in to the DockerHub
account to be able to push the result.

## Build image from Dockerfile

To build a new version of the clean_lean image, 
run the following command in a terminal with this
directory as the current working directory. The
repository image name is kevinsullivan/clean_lean.
It will have the tag, *latest*.

``` sh
docker build -t kevinsullivan/leanvm:latest . -m 8g
```

## Push image to DockerHub

To push a copy of this image to dockerhub, do this:

``` sh
docker push kevinsullivan/leanvm
```

## Pull image from DockerHub

To pull a copy of the image to your local host machine, run:

```sh
docker pull kevinsullivan/leanvm
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
    --name %container_name% -v %source_directory_on_host%:/dm kevinsullivan/leanvm \
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
