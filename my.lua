print "load lua"

runsteps=1001

function state(x,y,z,t)
    if x<310 and x>290 and y <810 and y>790 then
        return 1
    else
        return 0
    end
end

-- function getrate(x,y,z,frameid,eventid,cx,cy,cz)
--     -- print "getrate"
--     print(cx,cy,cz)
--     local rate={0.5,0.4,0.4,0.4,0.4,0.6,0.5,1.3,0.55,0.55,0.5,0.65,0.5,0.3}
--     return math.exp(-rate[eventid+1]*10)
-- end

function getrate(x,y,z,frameid,eventid,cx,cy,cz)
    -- print "getrate"
    --r=math.sqrt((cx-x)^2+(cy-y)^2+(cz-z)^2)
    if frameid == 0 then
        local rate={0.5,0.4,0.4,0.4,0.4,0.6,0.5,1.3,0.55,0.55,0.5,0.65,0.5,0.3}
        return math.exp(-rate[eventid+1]*10)
    else
        return math.exp(-0.5*10)
    end
end
