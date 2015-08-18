import wx
from matplotlib.figure import Figure
import matplotlib.font_manager as font_manager
import numpy as np
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

TIMER_ID = wx.NewId()
POINTS =100
class PlotFigure(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self,None,wx.ID_ANY,title="Macaca Power Consumption Monitor",size=(600,400))
        self.fig = Figure((6,4),100)
        self.canvas = FigureCanvas(self,wx.ID_ANY,self.fig)
        self.ax = self.fig.add_subplot(111)
        self.ax.set_ylim([0,100])
        self.ax.set_xlim([0,POINTS])

        self.ax.set_autoscale_on(False)
        self.ax.set_xticks(range(0,101,10))
        self.ax.set_yticks(range(0,101,10))
        self.ax.grid(True)
        self.user = [None]*POINTS
        self.user_2 = [None]*POINTS
        self.l_user,=self.ax.plot(range(POINTS),self.user,label='Power Consumption Ratio %')
        self.r_user,=self.ax.plot(range(POINTS),self.user_2,label='Job Finish Ratio %')

        self.ax.legend(loc='upper center',
                            ncol=4,
                            prop=font_manager.FontProperties(size=10))
        self.canvas.draw()
        self.bg = self.canvas.copy_from_bbox(self.ax.bbox)
        wx.EVT_TIMER(self, TIMER_ID, self.onTimer)
    def onTimer(self, evt):
        self.canvas.restore_region(self.bg)
        #temp = np.random.randint(0,100)
        temp = open('/proc/loadavg').read().strip().split()[:3]
        value = float(temp[1]) * 100
        print value
        value2 = float(temp[2]) * 1000
        self.user = self.user[1:]+[ int(value)]
        self.user_2 = self.user_2[1:]+[ int(value2)]
        self.l_user.set_ydata(self.user)
        self.r_user.set_ydata(self.user_2)
        self.ax.draw_artist(self.l_user)
        self.ax.draw_artist(self.r_user)
        self.canvas.blit(self.ax.bbox)
    def __del__( self ):
        t.Stop()
if __name__ == '__main__':
    app = wx.PySimpleApp()
    frame = PlotFigure()
    #frame2 = PlotFigure()
    t = wx.Timer(frame, TIMER_ID)
    t.Start(200)
    #t2 = wx.Timer(frame2, TIMER_ID)
    #t2.Start(100)
    frame.Show()
    #frame2.Show()
    app.MainLoop()
