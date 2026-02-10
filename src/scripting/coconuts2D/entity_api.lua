local entity = {}
entity.__index = entity

function entity:extend()
    local child = setmetatable({}, self)
    child.__index = child
    return child
end

function entity:has_component(component)
    return self.registry:has(self:id(), component)
end

function entity:get_component(component)
    return self.registry:get(self.id(), component)
end

function entity:remove_component(component)
    return self.registry:remove(self.id(), component)
end

return entity