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
		mv sdl_wine_combine_triggers.so sdl_wine_combine_triggers_$arch.so
	"
done
