#!/bin/bash -e

for DOCKERFILE in */Dockerfile
do
	DOCKERDIR=${DOCKERFILE///*}
	DOCKERTAG=angr-test:$DOCKERDIR
	echo "Building $DOCKERDIR"
	docker build -t $DOCKERTAG $DOCKERDIR
done
