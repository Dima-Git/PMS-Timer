sudo insmod timer_sysfs.ko

gnome-terminal -e "tail -f /var/log/kern.log"

echo "1sec timeout, 3sec sleep"
echo 1000 > /sys/timer_dir/timer_period
sleep 3

echo "0.5sec timeout, 3sec sleep"
echo 500 > /sys/timer_dir/timer_period
sleep 3

echo "0sec timeout, 3sec sleep"
echo 0 > /sys/timer_dir/timer_period
sleep 3

echo "2sec timeout, 3sec sleep"
echo 2000 > /sys/timer_dir/timer_period
sleep 3

sudo rmmod timer_sysfs
