local core = require("coconuts2D.core_api")
local components = core.components

local this = {}


function this:init()
    print("init() - id: " .. self.id())

    if core.has_component(self, components.TagComponent) then
        tagComponent = core.get_component(self, components.TagComponent)
        print("  Hello " .. tagComponent.tag)

        -- Update its tag
        tagComponent.tag = "Luís Vaz de Camões"
    end
end


function this:update(time)
    print("update() - id: " .. self.id() .. " with arg: " .. time)

    if core.has_component(self, components.TagComponent) then
        tagComponent = core.get_component(self, components.TagComponent)
        print("  Hello " .. tagComponent.tag)
    end
end


function this:destroy()
    print("destroy() - id: " .. self.id())
end

return this
