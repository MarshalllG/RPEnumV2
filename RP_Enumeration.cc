#include "RP_Enumeration.hh"

// // metodo della classe
// vector<char> MatrixToVector(vector<vector<char>> cells, unsigned rows, unsigned columns)
// {
//     unsigned dim = rows*columns;
//     vector<char> vect(dim);
//     unsigned i = 0;

//     while (i < dim)
//     {

//         for (unsigned r=0; r < rows; r++)
//         {
//             for (unsigned c=0; c < columns; c++)
//             {
//                 vect[i] = cells[r][c];
//             }
//         }
//         i++;
//     }

//     return vect;
// }

// // metodo della classe
// vector<vector<char>> VectorToMatrix(vector<char> vect, unsigned rows, unsigned columns)
// {
//     vector<vector<char>> cells;
//     unsigned i = 0;

//     while (i < rows*columns)
//     {

//         for (unsigned r=0; r < rows; r++)
//         {
//             for (unsigned c=0; c < columns; c++)
//             {
//                 cells[r][c] = vect[i];
//             }
//         }
//         i++;
//     }

//     return cells;
// }

// Insert here the code in the method of class EnumerationWLOpt
void EnumerationRPOpt::First()
{ // insert first router

    point router;
    router.row = 0;
    router.col = 0;

    while (in.IsWall(router.row, router.col))
    {
        if (router.col < in.Columns()-1)
            router.col++;
        else if (router.row < in.Rows()-1 && router.col == in.Columns()-1) 
            router.row++;
    }

    out.InsertRouter(router.row, router.col);
    out.ConnectRouterWithBackbone(router);
}


// bool EnumerationRPOpt::Next()
// {  
//     // get coordinates of last router 
//     unsigned r = out.Router(out.RoutersSize()-1).row;
//     unsigned c = out.Router(out.RoutersSize()-1).col;

//     cout << "last: " << r << " " << c << endl;

//     if (r == in.Rows()-1 && c == in.Columns()-1) 
//     {
//         cout << "next cycle" << endl;

//         unsigned i;
//         point first;

//         if (out.RoutersSize() == 1)
//         {
//             cout << "out.RoutersSize(): " << out.RoutersSize() << endl;
//             return false;
//         }

//         // get coordinates of first router 
//         for (first.row = 0; first.row < in.Rows(); first.row++)
//             for (first.col = 0; first.col < in.Columns(); first.col++)
//                 if (out.Cell(first.row, first.col) == 'r')
//                     break;

//         cout << "first: " << first.row << " " << first.col << endl;

//         // remove all routers
//         out.RemoveAllRoutersAndRestartBackbone();

//         r = first.row;
//         c = first.col;

//         // change first router in next iteration
//         do
//         {
//             c < in.Columns()-1 ? c++ : r++;
//         } while (in.IsWall(r,c));
           
//         out.InsertRouter(r,c);
//         out.UpdateBackbone();
//     }
    
//     if (c < in.Columns()-1) // NextInSameRow()
//     {
//         // out.RemoveRouter(out.Router(out.RoutersSize()-1));
//         out.RemoveLastRouter();
//         out.UpdateBackbone();

//         // skip walls
//         do
//         {
//             c++;
//         } while (in.IsWall(r,c));

//         out.InsertRouter(r,c);
//         out.UpdateBackbone();
//     }
//     else if (r < in.Rows()-1 && c == in.Columns()-1) // NextInNewRow()
//     {
//         // out.RemoveRouter(out.Router(out.RoutersSize()-1));
//         out.RemoveLastRouter();
//         out.UpdateBackbone();

//         c = 0;
//         r++;

//         // skip walls
//         while (in.IsWall(r,c))
//             c++;
        
//         out.InsertRouter(r,c);
//         out.UpdateBackbone();
//     }

//     cout << out;

//     return true;
// }
    
bool EnumerationRPOpt::Next()
{
    point router;
    unsigned i = 0, c1, k = 0, r = 0;

    cout << "Next: " << endl;
    cout << out << endl;

    for (unsigned r = 0; r < in.Rows(); r++)
    {
        for (unsigned c = 0; c < in.Columns(); c++)
        {
            cout << "r c: " << r << " " << c << endl;

            if (out.Cells()[r][c] == 'r')
            {
                router.col = c;
                router.row = r;

                if(in.IsGridPoint(r, c+1))
                {
                    if (out.Cells()[r][c+1] == 'r')
                    {
                        k++;
                    }
                    else
                    {   
                        out.RemoveRouter(router);
                        out.InsertRouter(r,c+1);
                        for (c1 = 0; c1 < k; c1++)
                        {
                            router.row = r;
                            router.col = c-k+c1;
                            out.RemoveRouter(router);
                            out.InsertRouter(r, c1);
                        }
                        return true;
                    }
                }
                else if(in.IsGridPoint(r+1, 0))
                {
                    c = -1;
                    if (out.Cells()[r+1][c+1] == 'r')
                    {
                        k++;
                    }
                    else
                    {  
                        out.RemoveRouter(router);
                        out.InsertRouter(r+1,c+1);
                        for (c1 = 0; c1 < k; c1++)
                        {
                            router.row = r;
                            router.col = c-k+c1;
                            out.RemoveRouter(router);
                            out.InsertRouter(r, c1);
                        }
                        return true;
                    }
                }
            }
        }
    }

    if (out.RoutersSize() == in.Rows()*in.Columns())
        return false;

    unsigned routers = out.RoutersSize();
    
    cout << "RemoveandRestart" << endl;
    cout << out;
    out.RemoveAllRoutersAndRestartBackbone();
    cout << "RemoveDone" << endl;
    cout << out;
    unsigned n = 0;
    for (unsigned r = 0; r < in.Rows(); r++)
    {
        for (unsigned c = 0; c < in.Columns(); c++)
        {
            if (n > routers)
                break;
            out.InsertRouter(r,c);
            n++;
        }
    }
    

    cout << "Inserimento router: " << endl;
    cout << out;
    cout << "Start with " << out.RoutersSize() << " routers" << endl;
    return true;
}


bool EnumerationRPOpt::Feasible()
{
    // check budget
    if (out.RemainingBudget() < 0)
        return false;

    // check routers
    for (unsigned i = 0; i < out.RoutersSize(); i++)
        if (in.IsWall(out.Router(i).row, out.Router(i).col))
            return false;
    
    return true;
}


int EnumerationRPOpt::Cost()
{
    // if (out.TotalCoveredPoints() == 0)
    //     return 0;
    // else
    //     return out.RoutersSize()*in.RouterPrice()+out.BackboneSize()*in.BackbonePrice()-out.TotalCoveredPoints();
}


