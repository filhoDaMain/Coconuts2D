local this = {}

function this:init()
    print("init() - id: " .. self.id())
end

function this:update(time)
    print("update() - id: " .. self.id() .. " with arg: " .. time)
end

function this:destroy()
    print("destroy() - id: " .. self.id())
end

return this
