print "load lua"
function state(x,y,z,t)
    if x<310 and x>300 and y <710 and y>690 then
        return 1
    else
        return 0
    end
end

function getrate(x,y,z,frameid,eventid,cx,cy,cz)
    print "getrate"
    local rate={0.5,0.4,0.4,0.4,0.4,0.6,0.5,1.3,0.55,0.55,0.5,0.65,0.5,0.3}
    return math.exp(-rate[eventid+1]*10)
end