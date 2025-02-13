local this = {}

function this:init()
    print("init() - id: " .. self.id())

    if self.registry:has(self.id(), TagComponent) then
        tagComponent = self.registry:get(self.id(), TagComponent)
        print("  Hello:  " .. tagComponent.tag)

        -- Update its tag
        tagComponent.tag = "Luís Vaz de Camões"
    end
end

function this:update(time)
    print("update() - id: " .. self.id() .. " with arg: " .. time)

    if self.registry:has(self.id(), TagComponent) then
        tagComponent = self.registry:get(self.id(), TagComponent)
        print("  Hello again:  " .. tagComponent.tag)
    end
end

function this:destroy()
    print("destroy() - id: " .. self.id())
end

return this
