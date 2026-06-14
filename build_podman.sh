set -xe

IMAGE_TAG=debian:bookworm

for arch in amd64 386
do
	podman run \
		--rm -it \
		--security-opt label=disable \
		-v ./:/work_dir \
		-w /work_dir \
		--entrypoint /bin/bash \
		--arch $arch \
		$IMAGE_TAG \
		-c "
		apt update
		apt install -y libsdl2-dev gcc g++
		bash build.sh
		mv nfsw_wine_controller_fix.so nfsw_wine_controller_fix_$arch.so
	"
done
