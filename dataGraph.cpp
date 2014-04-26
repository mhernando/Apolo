
#include "dataGraph.h"


DataGraph::DataGraph(wxWindow *parent, wxWindowID id,const wxString& title_frame, NodeTree* itemData,
							 NodeTree *parentData)
: wxFrame(parent, id, title_frame, wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE) 
{

}

DataGraph::~DataGraph()
{


}

template<class T> 
void DataGraph::enterData(TypeData _type, T data)
{


}



/*  real time function */

/*

void mpFXYVector::AddData(float x, float y, std::vector<double> &xs, std::vector<double> &ys)
    {
        // Check if the data vectora are of the same size
        if (xs.size() != ys.size()) {
            wxLogError(_("wxMathPlot error: X and Y vector are not of the same length!"));
            return;
        }

        //Delete first point if you need a filo buffer (i dont need it)
        //xs.erase(xs.begin());
        //xy.erase(xy.begin());

        //Add new Data points at the end
        xs.push_back(x);
        ys.push_back(y);


        // Copy the data:
        m_xs = xs;
        m_ys = ys;

        // Update internal variables for the bounding box.
        if (xs.size()>0)
        {
            m_minX  = xs[0];
            m_maxX  = xs[0];
            m_minY  = ys[0];
            m_maxY  = ys[0];

            std::vector<double>::const_iterator  it;

            for (it=xs.begin();it!=xs.end();it++)
            {
                if (*it<m_minX) m_minX=*it;
                if (*it>m_maxX) m_maxX=*it;
            }
            for (it=ys.begin();it!=ys.end();it++)
            {
                if (*it<m_minY) m_minY=*it;
                if (*it>m_maxY) m_maxY=*it;
            }
            m_minX-=0.5f;
            m_minY-=0.5f;
            m_maxX+=0.5f;
            m_maxY+=0.5f;
        }
        else
        {
            m_minX  = -1;
            m_maxX  = 1;
            m_minY  = -1;
            m_maxY  = 1;
        }
    }

*/