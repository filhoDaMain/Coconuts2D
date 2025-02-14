M = {

    components = {
        TagComponent = _G.TagComponent
    },

    has_component = function(self, component)
        return self.registry:has(self.id(), component)
    end,

    get_component = function(self, component)
        return self.registry:get(self.id(), component)
    end

}

return M
