local entity = require("coconuts2D.entity_api")
local this = entity


function this:init()
    print("Hello world!!!");
    print("Initing Entity # " .. this.id())

    if this:has_component(TagComponent) then
        local tag = this:get_component(TagComponent)
        print("  Name: " .. tag.tag)
    end

    if this:has_component(TransformComponent) then
        local transform = this:get_component(TransformComponent)
        print("  Pos.x = " .. transform.position.x)
        print("  Pos.y = " .. transform.position.y)

        -- Make an update to check at update() time
        transform.position.x = 3
        transform.position.y = 7
    end
end


function this:update(time)
    print("Updating Entity # " .. this.id())

    if this:has_component(TagComponent) then
        local tag = this:get_component(TagComponent)
        print("  Name: " .. tag.tag)
    end

    if this:has_component(TransformComponent) then
        local transform = this:get_component(TransformComponent)
        print("  Pos.x = " .. transform.position.x)
        print("  Pos.y = " .. transform.position.y)
    end
end


function this:destroy()
    print("Destroying Entity # " .. this.id())
end


return this
