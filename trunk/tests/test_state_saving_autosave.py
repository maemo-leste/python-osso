import unittest
import osso

class TestOssoStateSavingAutosave(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("app", "0.1")
        self.number = 3.1415
    
    def test_constructor_state_saving(self):
        state_saving = osso.StateSaving(self.ctx)
        self.assertTrue(isinstance(state_saving, osso.StateSaving))
        self.assertRaises(TypeError, osso.StateSaving, None)

    def test_constructor_autosave(self):
        autosave = osso.Autosave(self.ctx)
        self.assertTrue(isinstance(autosave, osso.Autosave))
        self.assertRaises(TypeError, osso.Autosave, None)

    def test_autosaving(self):
        state_saving = osso.StateSaving(self.ctx)
        #callback for autosave
        def save_state(self, user_data=None):
            t = "Unit test for the state saving and autosave OSSO modules"
            n = 3.1415
            state_saving.state_write((t,n))
        
        autosave = osso.Autosave(self.ctx)
        autosave.set_autosave_callback(save_state) 
        autosave.force_autosave()
        data = state_saving.state_read()
        saved_text = data[0]
        saved_number = data[1]
        self.assertEquals(saved_text,"Unit test for the state saving and autosave OSSO modules")
        self.assertEquals(saved_number,3.1415)
    
    def test_autosave_get_name(self):
        autosave = osso.Autosave(self.ctx)
        autosave.get_name()

    def test_autosave_get_version(self):
        autosave = osso.Autosave(self.ctx)
        autosave.get_version()

    def test_autosave_userdata_changed(self):
        autosave = osso.Autosave(self.ctx)
        autosave.userdata_changed()


if __name__ == "__main__":
    unittest.main()
