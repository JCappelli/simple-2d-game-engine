--Enemy Behaviour
return {
    on_start = function(entity)
        startingSpeed = 20

        print("start function being called, test 3")
        dir = 2 * math.pi * (math.random())

        vx = math.cos(dir) * startingSpeed
        vy = math.sin(dir) * startingSpeed

        set_velocity(entity, vx, vy);
    end,
    on_update = function(entity, delta_time)
    end
}

