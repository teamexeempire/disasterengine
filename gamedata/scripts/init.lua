local logo = sprite_load("logo.png", "default")
local mus = music_load("mus_fartzone_chase.ogg", "default")
local fade = 1

function tick(delta, realDelta)
    fade = fade - realDelta
    
    if(fade <= 0) then
        set_script("menu.lua", "default")
    end
end

function draw(delta, realDelta)    
    sprite_drawex(logo, 0, 0, 0, 255, 255, 255, fade * 255)
end

music_play(mus, true)
vsync_set(VSYNC_OFF)