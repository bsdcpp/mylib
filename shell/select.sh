#!/bin/bash

fgetopts()
{
	while getopts ":mnopq:rs" Option
	do
		case $Option in
			m     ) echo "Scenario #1: option -m-   [OPTIND=${OPTIND}]";;
			n | o ) echo "Scenario #2: option -$Option-   [OPTIND=${OPTIND}]";;
			p     ) echo "Scenario #3: option -p-   [OPTIND=${OPTIND}]";;
			q     ) echo "Scenario #4: option -q-\
			with argument \"$OPTARG\"   [OPTIND=${OPTIND}]";;
			#  注意,选项'q'必须分配一个参数,
			#+ 否则默认将失败.
			r | s ) echo "Scenario #5: option -$Option-";;
			*     ) echo "Unimplemented option chosen.";;   # DEFAULT
		esac
	done

	shift $(($OPTIND - 1))
	#  将参数指针减1,这样它将指向下一个参数.
	#  $1 现在引用的是命令行上的第一个非选项参数
	#+ 如果有一个这样的参数存在的话.

	exit 0
}

fgetopt()
{
	#!/bin/bash
	# 使用 getopt.

	# 尝试使用下边的不同的方法来调用这脚本:
	#   sh ex33a.sh -a
	#   sh ex33a.sh -abc
	#   sh ex33a.sh -a -b -c
	#   sh ex33a.sh -d
	#   sh ex33a.sh -dXYZ
	#   sh ex33a.sh -d XYZ
	#   sh ex33a.sh -abcd
	#   sh ex33a.sh -abcdZ
	#   sh ex33a.sh -z
	#   sh ex33a.sh a
	# 解释上面每一次调用的结果.

	E_OPTERR=65

	if [ "$#" -eq 0 ]
	then   # 脚本需要至少一个命令行参数.
		echo "Usage $0 -[options a,b,c]"
		exit $E_OPTERR
	fi  

	set -- `getopt "abcd:" "$@"`
	# 为命令行参数设置位置参数.
	# 如果使用 "$*" 来代替 "$@" 的话会发生什么?

	while [ ! -z "$1" ]
	do
		case "$1" in
			-a) echo "Option \"a\"";;
			-b) echo "Option \"b\"";;
			-c) echo "Option \"c\"";;
			-d) echo "Option \"d\" $2";;
			*) break;;
		esac

		shift
	done

	#  通常来说在脚本中使用内建的 'getopts' 命令,
	#+ 会比使用 'getopt' 好一些.
	#  参见 "ex33.sh".

	exit 0
}


PS3='Choose the command you wanna to run: ' # 设置提示符字串.

select vegetable in "fgetopts" "fgetopt"
do
	echo "=============="
	$vegetable $@
	echo "=============="
	break  # 如果这里没有'break'会发生什么?
done

exit 0
