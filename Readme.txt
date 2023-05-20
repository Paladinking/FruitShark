Running headless server + clients:
    Split tick in Shark, Ship, Bite (just remove Bite form server...), Pickup, etc into tick_physics and tick_io
    Client only calls tick_io, server only calls tick_physics
    Keep regular tick?
        Saves trouble with merging input schemes
        But input schemes have to change anyway soon to support controllers
    Send key / mouse down and up events to server (channel ?)
        First translate into neutral inputs
    Change positions to float
    Change angle to float
    Receive Game State from server (channel 0)
        All shark positions
        All ship positions
        All pickup positions
        Fruits in water? Do they still move?
        Send every 10 / 20 / 30 ms?
        3 * 4 bytes per shark, 3 * 4 bytes per ship, 2 * 4 bytes per pickup
            Pack better? Rendering only needs integer position
            1080 * 1920 = 2073600 < 2^32
            2048 * 2048 => 22 bits
            gives 10 bits for angle => 1024, ca 0.35 degree precision
            Fits into single int32
            This would mean 100 objects of any kind => 400 bytes
            0.5 Mbps = 65536 bytes / s => 400 bytes / 6ms
            16.666 ms gives 60 fps, allows 1048 bytes (not 4 players...)
            Could also compress data...
                That takes time but could be faster if network speed is limiting

    Receive

