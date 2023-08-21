--Enemy Behaviour
return {
    on_start = function(entity)
        startingSpeed = 20

        dir = 2 * math.pi * (math.random())

        vx = math.cos(dir) * startingSpeed
        vy = math.sin(dir) * startingSpeed

        set_velocity(entity, vx, vy);
    end,

    on_hit = function (this, other)
        local current_vel_x, current_vel_y = get_velocity(this);
        
        current_vel_x = current_vel_x * -1;
        current_vel_y = current_vel_y * -1;

        set_velocity(this, current_vel_x, current_vel_y);
    end
}

