#include <iostream>
#include <ctime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    renderArea = ui->renderArea;
    rrt = renderArea->rrt;
    simulated = false;
}

/**
 * @brief Start the simulator.
 */
void MainWindow::on_startButton_clicked()
{
    if (simulated) {
        ui->statusBox->setText(tr("Please reset!"));
        renderArea->update();
        return;
    }
    simulated = true;
    // get step size and max iterations from GUI.
    rrt->setMaxIterations(ui->maxIterations->text().toInt());
    rrt->setStepSize(ui->stepSize->text().toInt());
    std::cout << "[RRT]" << "start_pos: " << rrt->start_pos.transpose()
                         << ", end_pos: " << rrt->end_pos.transpose() << std::endl;

    assert(rrt->step_size > 0);
    assert(rrt->max_iter > 0);
    assert(rrt->start_pos.x() > 0.0);
    assert(rrt->start_pos.y() > 0.0);

    // RRT Algorithm
    clock_t start,end;
    start = clock();

    int iter_num = 0;
    for(iter_num = 0; iter_num < renderArea->rrt->max_iter; iter_num++) {
        Node *q = rrt->getRandomNode();
        if (q) {
            Node *qNearest = rrt->nearest(q->position);
            if (rrt->distance(q->position, qNearest->position) > rrt->step_size) {
                Vector2f newConfig = rrt->newConfig(q, qNearest);
                if (!rrt->obstacles->isSegmentInObstacle(newConfig, qNearest->position)) {
                    Node *qNew = new Node;
                    qNew->position = newConfig;
                    rrt->add(qNearest, qNew);
                }
            }
        }
        if (rrt->reached()) {
            ui->statusBox->setText(tr("Reached Destination!"));
            std::cout << "[rrt] rrt iter_num: " << iter_num << std::endl;
            break;
        }
    }
    Node *q;
    if (rrt->reached()) {
        q = rrt->lastNode;
    }else{
        // if not reached yet, then shortestPath will start from the closest node to end point.
        q = rrt->nearest(rrt->end_pos);
        ui->statusBox->setText(tr("Exceeded max iterations!"));
    }
    // generate the shortest path to destination.
    while (q != nullptr) {
        rrt->path.push_back(q);
        q = q->parent;
    }
    renderArea->update();
    end = clock();
    std::cout << "[rrt] cost time "<< double(end-start)/CLOCKS_PER_SEC * 1000 << "ms"<< std::endl;
}

/**
 * @brief Delete all obstacles, nodes and paths from the simulator.
 */
void MainWindow::on_resetButton_clicked()
{
    simulated = false;
    ui->statusBox->setText(tr(""));
    rrt->obstacles->obstacles.clear();
    rrt->obstacles->obstacles.resize(0);
    rrt->deleteNodes(rrt->root);
    rrt->nodes.clear();
    rrt->nodes.resize(0);
    rrt->path.clear();
    rrt->path.resize(0);
    rrt->initialize();
    renderArea->update();
}

/**
 * @brief Delete all obstacles, nodes and paths from the simulator.
 */
void MainWindow::on_resetPosButton_clicked()
{
    simulated = false;
    ui->statusBox->setText(tr(""));
    rrt->deleteNodes(rrt->root);
    rrt->nodes.clear();
    rrt->nodes.resize(0);
    rrt->path.clear();
    rrt->path.resize(0);
    rrt->initialize();
    renderArea->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
