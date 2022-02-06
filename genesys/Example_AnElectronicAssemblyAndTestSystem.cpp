//
// Created by gabriel on 12/14/21.
//

#include "Example_AnElectronicAssemblyAndTestSystem.h"

// GEnSyS Simulator
#include "../../kernel/simulator/Simulator.h"

// Model Components
#include "../../plugins/components/Create.h"
#include "../../plugins/components/Delay.h"
#include "../../plugins/components/Dispose.h"
#include "../../plugins/components/Seize.h"
#include "../../plugins/components/Release.h"
#include "../../plugins/components/Assign.h"
#include "../../plugins/components/Record.h"
#include "../../plugins/components/Decide.h"
#include "../../plugins/components/Write.h"

// Model elements
#include "../../kernel/simulator/ElementManager.h"
#include "../../kernel/simulator/EntityType.h"
#include "../../kernel/simulator/Attribute.h"
#include "../../plugins/elements/Variable.h"
#include "../../plugins/elements/EntityGroup.h"
#include "../../plugins/elements/Set.h"

Example_AnElectronicAssemblyAndTestSystem::Example_AnElectronicAssemblyAndTestSystem() {}

/*
 *  Uppercase vs Lowercase
 *  Decide
 *  SeizableItemRequest
 *  Record
 */

int Example_AnElectronicAssemblyAndTestSystem::main(int argc, char** argv) {
    Simulator* genesys = new Simulator();
    this->insertFakePluginsByHand(genesys);
    this->setDefaultTraceHandlers(genesys->getTracer());
    genesys->getTracer()->setTraceLevel(Util::TraceLevel::L5_event);
    Model* model = genesys->getModels()->newModel();
//    model->load("./models/Example_AnElectronicAssemblyAndTestSystem.txt");
//    model->show();
//    model->getSimulation()->start();

    // Model Infos
    ModelInfo* infos = model->getInfos();
    infos->setAnalystName("Gabriel Carneiro");
    infos->setProjectTitle("An Electronic Assembly and Test System");
    infos->setDescription("B14 Chap 04 Modeling Basic Operations ans Inputs. Example section 4.1");

    // Simulation Configuration
    ModelSimulation* simulation = model->getSimulation();
    simulation->setReplicationLength(8);
    simulation->setReplicationLengthTimeUnit(Util::TimeUnit::hour);
    simulation->setNumberOfReplications(100);
    simulation->setWarmUpPeriod(1);
    simulation->setWarmUpPeriodTimeUnit(Util::TimeUnit::hour);
    simulation->setReplicationReportBaseTimeUnit(Util::TimeUnit::minute);

    // Part A Arrive
    EntityType* Part_A = new EntityType(model, "Part_A");
    Create* Part_A_Arrive = new Create(model, "Part A Arrive");
    Part_A_Arrive->setEntityType(Part_A);
    Part_A_Arrive->setTimeBetweenCreationsExpression("EXPO(5)");
    Part_A_Arrive->setTimeUnit(Util::TimeUnit::minute);

    // Part B Arrive
    EntityType* Part_B = new EntityType(model, "Part_B");
    Create* Part_B_Arrive = new Create(model, "Part B Arrive");
    Part_B_Arrive->setEntityType(Part_B);
    Part_B_Arrive->setTimeBetweenCreationsExpression("EXPO(30)");
    Part_B_Arrive->setTimeUnit(Util::TimeUnit::minute);
//    Part_B_Arrive->setEntitiesPerCreation(4);

    // Attributes
    new Attribute(model, "Sealer_Time");
    new Attribute(model, "Arrive_Time");

    // Assign Part A Sealer and Arrive Time
    Assign* Assign_Part_A_Sealer_and_Arrive_Time = new Assign(model, "Assign Part A Sealer and Arrive Time");
    Assign::Assignment* Sealer_Time_A = new Assign::Assignment("Sealer_Time", "TRIA(1,3,4)");
    Assign::Assignment* Arrive_Time_A = new Assign::Assignment("Arrive_Time", "tnow");
    Assign_Part_A_Sealer_and_Arrive_Time->getAssignments()->insert(Sealer_Time_A);
    Assign_Part_A_Sealer_and_Arrive_Time->getAssignments()->insert(Arrive_Time_A);

    // Assign Part B Sealer and Arrive Time
    Assign* Assign_Part_B_Sealer_and_Arrive_Time = new Assign(model, "Assign Part B Sealer and Arrive Time");
    Assign::Assignment* Sealer_Time_B = new Assign::Assignment("Sealer_Time", "WEIB(2.5, 5.3)");
    Assign::Assignment* Arrive_Time_B = new Assign::Assignment("Arrive_Time", "tnow");
    Assign_Part_B_Sealer_and_Arrive_Time->getAssignments()->insert(Sealer_Time_B);
    Assign_Part_B_Sealer_and_Arrive_Time->getAssignments()->insert(Arrive_Time_B);

    // Prep A Process - SeizeDelayRelease
    Resource* Prep_A = new Resource(model, "Prep A");
    Prep_A->setCapacity(1);

    Queue* Queue_Prep_A = new Queue(model, "Queue Prep A");
    Queue_Prep_A->setOrderRule(Queue::OrderRule::FIFO);

    Seize* Prep_A_Process_Seize = new Seize(model, "Prep A Process Seize");
    Prep_A_Process_Seize->getSeizeRequests()->insert(new SeizableItemRequest(Prep_A));
    Prep_A_Process_Seize->setQueue(Queue_Prep_A);

    Delay* Prep_A_Process_Delay = new Delay(model, "Prep A Process Delay");
    Prep_A_Process_Delay->setDelayExpression("TRIA(1, 4, 8)");
    Prep_A_Process_Delay->setDelayTimeUnit(Util::TimeUnit::minute);

    Release* Prep_A_Process_Release = new Release(model, "Prep A Process Release");
    Prep_A_Process_Release->getReleaseRequests()->insert(new SeizableItemRequest(Prep_A));

    // Prep B Process - SeizeDelayRelease
    Resource* Prep_B = new Resource(model, "Prep B");
    Prep_B->setCapacity(1);

    Queue* Queue_Prep_B = new Queue(model, "Queue Prep B");
    Queue_Prep_B->setOrderRule(Queue::OrderRule::FIFO);

    Seize* Prep_B_Process_Seize = new Seize(model, "Prep B Process Seize");
    Prep_B_Process_Seize->getSeizeRequests()->insert(new SeizableItemRequest(Prep_B));
    Prep_B_Process_Seize->setQueue(Queue_Prep_B);

    Delay* Prep_B_Process_Delay = new Delay(model, "Prep B Process Delay");
    Prep_B_Process_Delay->setDelayExpression("TRIA(3, 5, 10)");
    Prep_B_Process_Delay->setDelayTimeUnit(Util::TimeUnit::minute);

    Release* Prep_B_Process_Release = new Release(model, "Prep B Process Release");
    Prep_B_Process_Release->getReleaseRequests()->insert(new SeizableItemRequest(Prep_B));

    // Sealer Process - SeizeDelayRelease
    Resource* Sealer = new Resource(model, "Sealer");
    Sealer->setCapacity(1);

    Queue* Queue_Sealer = new Queue(model, "Queue Sealer");
    Queue_Sealer->setOrderRule(Queue::OrderRule::FIFO);

    Seize* Sealer_Process_Seize = new Seize(model, "Sealer Process Seize");
    Sealer_Process_Seize->getSeizeRequests()->insert(new SeizableItemRequest(Sealer));
    Sealer_Process_Seize->setQueue(Queue_Sealer);

    Delay* Sealer_Process_Delay = new Delay(model, "Sealer Process Delay");
    Sealer_Process_Delay->setDelayExpression("Sealer_Time");
    Sealer_Process_Delay->setDelayTimeUnit(Util::TimeUnit::minute);

    Release* Sealer_Process_Release = new Release(model, "Sealer Process Release");
    Sealer_Process_Release->getReleaseRequests()->insert(new SeizableItemRequest(Sealer));

    // Failed Sealer Inspection - Decide
    Decide* Failed_Sealer_Inspection = new Decide(model, "Failed Sealer Inspection");
    Failed_Sealer_Inspection->getConditions()->insert("UNIF(0,1) < 0.09");

    // Rework Process - SeizeDelayRelease
    Resource* Rework = new Resource(model, "Rework");
    Sealer->setCapacity(1);

    Queue* Queue_Rework = new Queue(model, "Queue Rework");
    Queue_Sealer->setOrderRule(Queue::OrderRule::FIFO);

    Seize* Rework_Process_Seize = new Seize(model, "Rework Process Seize");
    Rework_Process_Seize->getSeizeRequests()->insert(new SeizableItemRequest(Rework));
    Rework_Process_Seize->setQueue(Queue_Rework);

    Delay* Rework_Process_Delay = new Delay(model, "Rework Process Delay");
    Rework_Process_Delay->setDelayExpression("EXPO(45)");
    Rework_Process_Delay->setDelayTimeUnit(Util::TimeUnit::minute);

    Release* Rework_Process_Release = new Release(model, "Rework Process Release");
    Rework_Process_Release->getReleaseRequests()->insert(new SeizableItemRequest(Rework));

    // Failed Sealer Inspection - Decide
    Decide* Failed_Rework_Inspection = new Decide(model, "Failed Rework Inspection");
    Failed_Rework_Inspection->getConditions()->insert("UNIF(0,1) < 0.2");

    // Record Scrapped Parts
    Record* Record_Scrapped_Parts = new Record(model, "Record Scrapped Parts");
    Record_Scrapped_Parts->setExpressionName("Scrapped Parts Total Time");
    Record_Scrapped_Parts->setExpression("TNOW - Arrive_Time");

    // Record Salvaged Parts
    Record* Record_Salvaged_Parts = new Record(model, "Record Salvaged Parts");
    Record_Salvaged_Parts->setExpressionName("Salvaged Parts Total Time");
    Record_Salvaged_Parts->setExpression("TNOW - Arrive_Time");

    // Record Shipped Parts
    Record* Record_Shipped_Parts = new Record(model, "Record Shipped Parts");
    Record_Shipped_Parts->setExpressionName("Shipped Parts Total Time");
    Record_Shipped_Parts->setExpression("TNOW - Arrive_Time");

    // Scrapped Dispose
    Dispose* Scrapped = new Dispose(model, "Scrapped");

    // Salvaged Dispose
    Dispose* Salvaged = new Dispose(model, "Salvaged");

    // Shipped Dispose
    Dispose* Shipped = new Dispose(model, "Shipped");

    /************************* Model connections *******************************************/
    Part_A_Arrive->getNextComponents()->insert(Assign_Part_A_Sealer_and_Arrive_Time);
    Assign_Part_A_Sealer_and_Arrive_Time->getNextComponents()->insert(Prep_A_Process_Seize);
    Prep_A_Process_Seize->getNextComponents()->insert(Prep_A_Process_Delay);
    Prep_A_Process_Delay->getNextComponents()->insert(Prep_A_Process_Release);
    Prep_A_Process_Release->getNextComponents()->insert(Sealer_Process_Seize);

    Part_B_Arrive->getNextComponents()->insert(Assign_Part_B_Sealer_and_Arrive_Time);
    Assign_Part_B_Sealer_and_Arrive_Time->getNextComponents()->insert(Prep_B_Process_Seize);
    Prep_B_Process_Seize->getNextComponents()->insert(Prep_B_Process_Delay);
    Prep_B_Process_Delay->getNextComponents()->insert(Prep_B_Process_Release);
    Prep_B_Process_Release->getNextComponents()->insert(Sealer_Process_Seize);

    Sealer_Process_Seize->getNextComponents()->insert(Sealer_Process_Delay);
    Sealer_Process_Delay->getNextComponents()->insert(Sealer_Process_Release);
    Sealer_Process_Release->getNextComponents()->insert(Failed_Sealer_Inspection);

    Failed_Sealer_Inspection->getNextComponents()->insert(Rework_Process_Seize);
    Failed_Sealer_Inspection->getNextComponents()->insert(Record_Shipped_Parts);

    Rework_Process_Seize->getNextComponents()->insert(Rework_Process_Delay);
    Rework_Process_Delay->getNextComponents()->insert(Rework_Process_Release);
    Rework_Process_Release->getNextComponents()->insert(Failed_Rework_Inspection);

    Failed_Rework_Inspection->getNextComponents()->insert(Record_Scrapped_Parts);
    Failed_Rework_Inspection->getNextComponents()->insert(Record_Salvaged_Parts);

    Record_Scrapped_Parts->getNextComponents()->insert(Scrapped);
    Record_Salvaged_Parts->getNextComponents()->insert(Salvaged);
    Record_Shipped_Parts->getNextComponents()->insert(Shipped);

    // Save model and start simulation.
    model->save("./models/Test.txt");
    model->getSimulation()->start();

    return 0;
}