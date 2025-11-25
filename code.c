/*
 Insurance Premium Management System (C)
 Author: ChatGPT (generated for user)
 File: insurance.c
 Compile: gcc -o insurance insurance.c
 Run: ./insurance

 Features:
  - Add customer/policy
  - Display all customers
  - Search by ID
  - Update customer
  - Delete customer
  - Calculate & show premium for a policy
  - Save/load persistent data from "customers.dat"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "customers.dat"
#define NAME_LEN 100

typedef enum {
    POLICY_LIFE = 1,
    POLICY_HEALTH,
    POLICY_AUTO
} PolicyType;

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    int policy_type; /* 1-Life,2-Health,3-Auto */
    double sum_insured;
    int vehicle_age; /* only for auto; 0 otherwise */
} Customer;

/* Dynamic array of customers */
Customer *customers = NULL;
size_t cust_count = 0;

void strip_newline(char *s) {
    size_t L = strlen(s);
    if (L > 0 && s[L-1] == '\n') s[L-1] = '\0';
}

void load_data() {
    FILE *f = fopen(DATA_FILE, "rb");
    if (!f) {
        /* no file yet */
        return;
    }
    /* get file size and number of records */
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (size <= 0) { fclose(f); return; }
    cust_count = size / sizeof(Customer);
    customers = malloc(cust_count * sizeof(Customer));
    if (!customers) {
        printf("Memory allocation failed while loading data.\n");
        fclose(f);
        return;
    }
    size_t read = fread(customers, sizeof(Customer), cust_count, f);
    if (read != cust_count) {
        printf("Warning: read %zu of %zu records.\n", read, cust_count);
        cust_count = read;
    }
    fclose(f);
}

void save_data() {
    if (cust_count == 0) {
        /* remove file if no records */
        remove(DATA_FILE);
        free(customers);
        customers = NULL;
        return;
    }
    FILE *f = fopen(DATA_FILE, "wb");
    if (!f) {
        printf("Error: could not open data file for writing.\n");
        return;
    }
    fwrite(customers, sizeof(Customer), cust_count, f);
    fclose(f);
}

/* find index by ID, return -1 if not found */
int find_index_by_id(int id) {
    for (size_t i = 0; i < cust_count; ++i) {
        if (customers[i].id == id) return (int)i;
    }
    return -1;
}

int next_id() {
    int max = 0;
    for (size_t i = 0; i < cust_count; ++i)
        if (customers[i].id > max) max = customers[i].id;
    return max + 1;
}

double calculate_premium(const Customer *c) {
    /* Basic sample premium logic (simple, change as needed) */
    double premium = 0.0;
    double age_factor = 0.0;

    if (c->age < 30) age_factor = 0.0;
    else if (c->age < 45) age_factor = 0.005;  /* +0.5% */
    else if (c->age < 60) age_factor = 0.01;   /* +1% */
    else age_factor = 0.02;                    /* +2% */

    switch (c->policy_type) {
        case POLICY_LIFE:
            /* life: base 2% of sum insured + age factor */
            premium = c->sum_insured * (0.02 + age_factor);
            break;
        case POLICY_HEALTH:
            /* health: base 3% + higher age sensitivity */
            premium = c->sum_insured * (0.03 + age_factor * 1.5);
            break;
        case POLICY_AUTO: {
            /* auto: base 5% + vehicle age factor */
            double vfactor = 0.0;
            if (c->vehicle_age <= 2) vfactor = 0.0;
            else if (c->vehicle_age <= 5) vfactor = 0.005;
            else vfactor = 0.01;
            premium = c->sum_insured * (0.05 + age_factor + vfactor);
            break;
        }
        default:
            premium = 0.0;
    }
    /* round to two decimals */
    premium = ((double)((int)(premium * 100 + 0.5))) / 100.0;
    return premium;
}

void add_customer() {
    Customer tmp;
    char buf[256];
    printf("\n--- Add Customer / Policy ---\n");
    tmp.id = next_id();

    printf("Full name: ");
    fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    strncpy(tmp.name, buf, NAME_LEN-1); tmp.name[NAME_LEN-1] = '\0';

    printf("Age: ");
    fgets(buf, sizeof(buf), stdin); tmp.age = atoi(buf);

    printf("Policy type (1-Life, 2-Health, 3-Auto): ");
    fgets(buf, sizeof(buf), stdin); tmp.policy_type = atoi(buf);
    if (tmp.policy_type < 1 || tmp.policy_type > 3) {
        printf("Invalid policy type. Cancelling add.\n");
        return;
    }

    printf("Sum insured (numeric, e.g. 500000): ");
    fgets(buf, sizeof(buf), stdin); tmp.sum_insured = atof(buf);

    tmp.vehicle_age = 0;
    if (tmp.policy_type == POLICY_AUTO) {
        printf("Vehicle age (years): ");
        fgets(buf, sizeof(buf), stdin); tmp.vehicle_age = atoi(buf);
    }

    /* append to array */
    Customer *newarr = realloc(customers, (cust_count + 1) * sizeof(Customer));
    if (!newarr) {
        printf("Memory allocation failed. Could not add customer.\n");
        return;
    }
    customers = newarr;
    customers[cust_count] = tmp;
    cust_count++;
    save_data();
    printf("Customer added with ID %d\n", tmp.id);
}

void display_customer(const Customer *c) {
    printf("ID: %d | Name: %s | Age: %d | Type: %s | Sum Insured: %.2f",
           c->id, c->name, c->age,
           (c->policy_type == POLICY_LIFE) ? "Life" :
           (c->policy_type == POLICY_HEALTH) ? "Health" : "Auto",
           c->sum_insured);
    if (c->policy_type == POLICY_AUTO) {
        printf(" | Vehicle age: %d", c->vehicle_age);
    }
    double prem = calculate_premium(c);
    printf(" | Premium: %.2f\n", prem);
}

void display_all() {
    printf("\n--- All Customers / Policies ---\n");
    if (cust_count == 0) {
        printf("No records found.\n");
        return;
    }
    for (size_t i = 0; i < cust_count; ++i) {
        display_customer(&customers[i]);
    }
}

void search_by_id() {
    char buf[64];
    printf("\nEnter customer ID to search: ");
    fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    int idx = find_index_by_id(id);
    if (idx < 0) {
        printf("Customer with ID %d not found.\n", id);
        return;
    }
    display_customer(&customers[idx]);
}

void update_customer() {
    char buf[256];
    printf("\nEnter customer ID to update: ");
    fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    int idx = find_index_by_id(id);
    if (idx < 0) {
        printf("Customer with ID %d not found.\n", id);
        return;
    }
    Customer *c = &customers[idx];
    printf("Updating customer (leave blank to keep current)\n");
    printf("Current name: %s\nNew name: ", c->name);
    fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    if (strlen(buf) > 0) strncpy(c->name, buf, NAME_LEN-1);

    printf("Current age: %d\nNew age: ", c->age);
    fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    if (strlen(buf) > 0) c->age = atoi(buf);

    printf("Current policy type: %d (1-Life,2-Health,3-Auto)\nNew policy type: ", c->policy_type);
    fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    if (strlen(buf) > 0) c->policy_type = atoi(buf);

    printf("Current sum insured: %.2f\nNew sum insured: ", c->sum_insured);
    fgets(buf, sizeof(buf), stdin); strip_newline(buf);
    if (strlen(buf) > 0) c->sum_insured = atof(buf);

    if (c->policy_type == POLICY_AUTO) {
        printf("Current vehicle age: %d\nNew vehicle age: ", c->vehicle_age);
        fgets(buf, sizeof(buf), stdin); strip_newline(buf);
        if (strlen(buf) > 0) c->vehicle_age = atoi(buf);
    } else {
        c->vehicle_age = 0;
    }
    save_data();
    printf("Customer updated.\n");
}

void delete_customer() {
    char buf[64];
    printf("\nEnter customer ID to delete: ");
    fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    int idx = find_index_by_id(id);
    if (idx < 0) {
        printf("Customer with ID %d not found.\n", id);
        return;
    }
    /* shift */
    for (size_t i = idx; i + 1 < cust_count; ++i) {
        customers[i] = customers[i+1];
    }
    cust_count--;
    if (cust_count == 0) {
        free(customers);
        customers = NULL;
    } else {
        customers = realloc(customers, cust_count * sizeof(Customer));
    }
    save_data();
    printf("Customer deleted.\n");
}

void calculate_premium_menu() {
    char buf[64];
    printf("\nEnter customer ID to calculate premium: ");
    fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    int idx = find_index_by_id(id);
    if (idx < 0) {
        printf("Customer with ID %d not found.\n", id);
        return;
    }
    Customer *c = &customers[idx];
    double prem = calculate_premium(c);
    printf("Calculated premium for ID %d (%s): %.2f\n", c->id, c->name, prem);
}

void pause_console() {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    getchar();
}

void show_menu() {
    printf("\n====== Insurance Premium Management System ======\n");
    printf("1. Add customer/policy\n");
    printf("2. Display all customers\n");
    printf("3. Search customer by ID\n");
    printf("4. Update customer\n");
    printf("5. Delete customer\n");
    printf("6. Calculate premium for a customer\n");
    printf("7. Exit\n");
    printf("Choose option: ");
}

int main() {
    load_data();
    char buf[16];
    int opt = 0;
    while (1) {
        show_menu();
        if (!fgets(buf, sizeof(buf), stdin)) break;
        opt = atoi(buf);
        switch (opt) {
            case 1: add_customer(); pause_console(); break;
            case 2: display_all(); pause_console(); break;
            case 3: search_by_id(); pause_console(); break;
            case 4: update_customer(); pause_console(); break;
            case 5: delete_customer(); pause_console(); break;
            case 6: calculate_premium_menu(); pause_console(); break;
            case 7: save_data(); printf("Exiting... Goodbye!\n"); exit(0);
            default: printf("Invalid option. Try again.\n"); pause_console();
        }
    }
    save_data();
    return 0;
}