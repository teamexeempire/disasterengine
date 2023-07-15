sprite = sprite_load("test.png");
track = music_load("test.ogg");
music_play(track)

function tick(delta)
    
end

function draw(delta)
    sprite_draw(sprite, 0, 0)
end