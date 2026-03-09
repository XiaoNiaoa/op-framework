# 一些实用封装好的函数

没什么特别的

## sound.inc

* OPCore_SoundPlay(soundid, Float:range, Float:x, Float:y, Float:z, worldid = 0, ints = 0)
    - 根据范围和位置播放声音 worldid/ints 为 -1的话则忽略虚拟世界或内饰

* OPCore_SoundStop(playerid, interval = 0)
    - 可以定时停止声音播放